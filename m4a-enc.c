#include <libavformat/avformat.h>
#include <aacenc_lib.h>
#include <string.h>
#include "getopt.h"
#include "wavreader.h"

static void usage(void)
{
    printf(
"Usage: m4a-enc [options] input_file\n"
"Options:\n"
" -h, --help                    Print this help message\n"
" -p, --profile <n>             Profile (audio object type)\n"
"                                 2: MPEG-4 AAC LC (default)\n"
"                                 5: MPEG-4 HE-AAC (SBR)\n"
"                                29: MPEG-4 HE-AAC v2 (SBR+PS)\n"
"                                23: MPEG-4 AAC LD\n"
"                                39: MPEG-4 AAC ELD\n"
" -b, --bitrate <n>             Bitrate in bits per seconds (for CBR)\n"
" -m, --bitrate-mode <n>        Bitrate configuration\n"
"                                 0: CBR (default)\n"
"                                 1-5: VBR\n"
"                               (VBR mode is not officially supported, and\n"
"                                works only on a certain combination of\n"
"                                parameter settings, sample rate, and\n"
"                                channel configuration)\n"
" -a, --afterburner <n>         Afterburner\n"
"                                 0: Off\n"
"                                 1: On(default)\n"
" -L, --lowdelay-sbr <-1|0|1>   Configure SBR activity on AAC ELD\n"
"                                -1: Use ELD SBR auto configurator\n"
"                                 0: Disable SBR on ELD (default)\n"
"                                 1: Enable SBR on ELD\n"
" -s, --sbr-ratio <0|1|2>       Controls activation of downsampled SBR\n"
"                                 0: Use lib default (default)\n"
"                                 1: downsampled SBR (default for ELD+SBR)\n"
"                                 2: dual-rate SBR (default for HE-AAC)\n"
"\n"
" -o <filename>                 Output filename\n"
);
}

#define WAVE_FORMAT_PCM 1

#define TRACE_ERR(cond, fmt, ...) if (cond) { fprintf(stderr, __FILE__ "(%u): " fmt "\n", __LINE__, ##__VA_ARGS__); return 1; }

static int MP4W_CreateContext(AVFormatContext** octx)
{
	int ret = avformat_alloc_output_context2(octx, NULL, "mp4" /*"mov"*/, NULL);
	TRACE_ERR(ret < 0, "Could not create output context: %s", av_err2str(ret))

	av_dict_set(&(*octx)->metadata, "encoder","AAC-FDK", 0);

	return 0;
}
static int MP4W_CreateStream(AVFormatContext* octx, int *stream_idx, int kbps, int sample_rate, int channels, void* dsi, unsigned dsi_size)
{
	int ret = 0;
	AVCodecContext* codecCtx = avcodec_alloc_context3( // release required?
		&(struct AVCodec) {
			.name = "L2HC-AAC",
			.type = AVMEDIA_TYPE_AUDIO,
			.id = AV_CODEC_ID_AAC,
		}
	);
	TRACE_ERR(!codecCtx, "Could not allocate codec context: %s", av_err2str(ret))
	codecCtx->bit_rate = kbps*1000;
	codecCtx->sample_rate = sample_rate;
	codecCtx->channel_layout = channels == 1 ? AV_CH_LAYOUT_MONO : AV_CH_LAYOUT_STEREO;
	codecCtx->frame_size = 1024; // ?
	codecCtx->channels = av_get_channel_layout_nb_channels(codecCtx->channel_layout);
	if(dsi) {
		codecCtx->extradata = dsi;
		codecCtx->extradata_size = (signed)dsi_size;
	}

	// Create a new audio stream in the output file container.
	AVStream* stream = avformat_new_stream(octx, NULL);
	TRACE_ERR(!stream, "Could not create new stream")

	stream->time_base = (AVRational){ 1, codecCtx->sample_rate };

	av_dict_set(&stream->metadata, "handler_name","AAC", 0);

	// copy the stream parameters to the muxer
	ret = avcodec_parameters_from_context(stream->codecpar, codecCtx);
	TRACE_ERR(ret < 0, "Could not copy the stream parameters: %s", av_err2str(ret))

	*stream_idx = stream->index;
	return 0;
}
static int MP4W_OpenFile(AVFormatContext* octx, const char* filename)
{
	int ret = 0;

	ret = avio_open(&octx->pb, filename, AVIO_FLAG_WRITE);
	TRACE_ERR(ret < 0, "Could not open '%s': %s", filename, av_err2str(ret))

	ret = avformat_write_header(octx, &octx->metadata);
	TRACE_ERR(ret < 0, "Error opening output file: %s", av_err2str(ret))

	return 0;
}
static int MP4W_Close(AVFormatContext* octx)
{
	int ret = 0;

	ret = av_write_trailer(octx);
	TRACE_ERR(ret < 0, "Error writing trailer: %s", av_err2str(ret))

	avio_closep(&octx->pb);
	avformat_free_context(octx);

	return 0;
}
static int MP4W_WritePacket(AVFormatContext* octx, int stream_idx, void* payload, int payload_size, int64_t pts, int64_t duration)
{
	int ret = 0;

	AVPacket* pkt = av_packet_alloc();
	TRACE_ERR(!pkt, "Failed to prepare test packet: %s", av_err2str(ret))

	ret = av_new_packet(pkt, payload_size);
	TRACE_ERR(ret < 0, "Failed to allocate payload data: %s", av_err2str(ret))
	pkt->stream_index = stream_idx;
	memcpy(pkt->data, payload, payload_size);
	pkt->pts = pts;
	pkt->dts = pkt->pts;
	pkt->duration = duration;
	ret = av_write_frame(octx, pkt);
	TRACE_ERR(ret < 0, "Failed writing output packet: %s", av_err2str(ret))

	av_packet_unref(pkt);

	return 0;
}

int main(int argc, char *argv[])
{
    static const struct option long_options[] = {
        { "help",             no_argument,       0, 'h' },
        { "profile",          required_argument, 0, 'p' },
        { "bitrate",          required_argument, 0, 'b' },
        { "bitrate-mode",     required_argument, 0, 'm' },
        { "afterburner",      required_argument, 0, 'a' },
        { "lowdelay-sbr",     required_argument, 0, 'L' },
        { "sbr-ratio",        required_argument, 0, 's' },
        { 0,                  0,                 0, 0   },
    };

	int ch, profile = 2, bitrate = 0, bitrate_mode = 0, afterburner = 1, lowdelay_sbr = 0 ,sbr_ratio = 0;
	const char *output_filename = NULL;
    while ((ch = getopt_long(argc, argv, "hp:b:m:a:L:s:o:", long_options, 0)) != EOF) {
        switch (ch) {
			int n;
			case 'h':
				return usage(), -1;
			case 'p':
				if (sscanf(optarg, "%u", &n) != 1) {
					TRACE_ERR(1, "invalid arg for profile: %s", optarg)
				}
				profile = n;
				break;
			case 'b':
				if (sscanf(optarg, "%u", &n) != 1) {
					TRACE_ERR(1, "invalid arg for bitrate: %s", optarg)
				}
				bitrate = n;
				break;
			case 'm':
				if (sscanf(optarg, "%u", &n) != 1 || n > 5) {
					TRACE_ERR(1, "invalid arg for bitrate-mode: %s", optarg)
				}
				bitrate_mode = n;
				break;
			case 'a':
				if (sscanf(optarg, "%u", &n) != 1 || n > 1) {
					TRACE_ERR(1, "invalid arg for afterburner: %s", optarg)
				}
				afterburner = n;
				break;
			case 'L':
				if (sscanf(optarg, "%d", &n) != 1 || n < -1 || n > 1) {
					TRACE_ERR(1, "invalid arg for lowdelay-sbr: %s", optarg)
				}
				lowdelay_sbr = n;
				break;
			case 's':
				if (sscanf(optarg, "%u", &n) != 1 || n > 2) {
					TRACE_ERR(1, "invalid arg for sbr-ratio: %s", optarg)
				}
				sbr_ratio = n;
				break;
			case 'o':
				output_filename = optarg;
				break;
			default:
				return usage(), 1;
		}
	}
    if (argc == optind)
        return usage(), 1;

    const char* input_filename = argv[optind];

    if (bitrate_mode == 0) {
		TRACE_ERR(bitrate <= 0, "incorrect bitrate for CBR mode: %d", bitrate)
		if (bitrate < 10000) {
			fprintf(stderr, "warn: consider input bitrate as given in kbps units\n");
			bitrate *= 1000;
		}
    }
	TRACE_ERR(output_filename == NULL, "output file name required")

	void *wavfile = wav_read_open(input_filename);
	TRACE_ERR(!wavfile, "can't open for reading: %s", output_filename)

	int format, channels, sample_rate, bits_per_sample;
	unsigned data_length;
	if (0 == wav_get_header(wavfile, &format, &channels, &sample_rate, &bits_per_sample, &data_length)) {
		fprintf(stderr, "can't read wav header: %s\n", output_filename);
		return 1;
	}
	TRACE_ERR(format != WAVE_FORMAT_PCM, "Unsupported WAV format other than PCM: %d", format)
	TRACE_ERR(bits_per_sample != 16, "Unsupported WAV sample depth: %d", bits_per_sample) // TODO
	TRACE_ERR(channels != 1 && channels != 2, "Unsupported WAV channels: %d", channels)

	HANDLE_AACENCODER enc;
	TRACE_ERR(AACENC_OK != aacEncOpen(&enc, 0, channels), "Unable to open encoder")

	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_CHANNELMODE, channels == 1 ? MODE_1 : MODE_2), "Unable to set the channel mode")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_CHANNELORDER, 1), "Unable to set the channel order")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_SAMPLERATE, sample_rate), "Unable to set the sampling rate %d", sample_rate)
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_AOT, profile), "Unable to set the AOT")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_SBR_MODE, lowdelay_sbr), "Unable to set SBR mode for ELD")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_SBR_RATIO, sbr_ratio), "Unable to set SBR ratio %d", sbr_ratio)
    TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_TRANSMUX, 0), "Unsupported transport format - RAW")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_SIGNALING_MODE, 1), "Failed to set Explicit backward compatible SBR signaling mode")
	TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_AFTERBURNER, afterburner), "Unable to set the afterburner mode %d", afterburner)
    if (bitrate_mode == 0) {
		TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_BITRATE, bitrate), "Unsupported bitrate %d", bitrate)
	} else {
		TRACE_ERR(AACENC_OK != aacEncoder_SetParam(enc, AACENC_BITRATEMODE, bitrate_mode), "Unsupported bitrate mode %d", bitrate_mode)
    }

	TRACE_ERR(AACENC_OK != aacEncEncode(enc, NULL, NULL, NULL, NULL), "Unable to initialize the encoder")
	AACENC_InfoStruct info = { 0 };
	TRACE_ERR(AACENC_OK != aacEncInfo(enc, &info), "Unable to get the encoder info");

	const int pcmBytes = info.frameLength*channels*sizeof(short);
	uint8_t *pcmBufBytes = malloc(pcmBytes);
	const int outputBytesMax = 6144 / 8 * channels;
	uint8_t *outputBuf = malloc(outputBytesMax);

	AVFormatContext* octx;
	TRACE_ERR(MP4W_CreateContext(&octx), "Can't create mp4 writer")

	int stream_idx;
	TRACE_ERR(MP4W_CreateStream(octx, &stream_idx, bitrate/1000, sample_rate, channels, info.confBuf, info.confSize), "Can't create stream")

	TRACE_ERR(MP4W_OpenFile(octx, output_filename), "Can't open for writing %s", output_filename)

	int64_t pts = 0;
	while(1) {
		int numReadBytes = wav_read_data(wavfile, pcmBufBytes, pcmBytes);

		AACENC_BufDesc bufIn = { 
			.numBufs = 1,
			.bufs = &pcmBufBytes,
			.bufferIdentifiers = &(int){IN_AUDIO_DATA},
			.bufSizes = &numReadBytes,
			.bufElSizes = &(int){sizeof(short)},
		};
		AACENC_InArgs argsIn = {
			.numInSamples = numReadBytes <= 0 ? -1 : numReadBytes/2
		};
		
		AACENC_BufDesc bufOut = { 
			.numBufs = 1,
			.bufs = &(outputBuf),
			.bufferIdentifiers = &(int){OUT_BITSTREAM_DATA},
			.bufSizes = &(int){outputBytesMax},
			.bufElSizes = &(int){1},
		};
		AACENC_OutArgs argsOut = { 0 };

		int encStatus = aacEncEncode(enc, &bufIn, &bufOut, &argsIn, &argsOut);
		if(encStatus != AACENC_OK && encStatus != AACENC_ENCODE_EOF) {
			TRACE_ERR(AACENC_OK != encStatus, "Encoding failed")
		}
		
		int64_t duration = info.frameLength;
		TRACE_ERR(MP4W_WritePacket(octx, stream_idx, outputBuf, argsOut.numOutBytes, pts, duration), "Packet write failed")
		pts += duration;

		if(encStatus == AACENC_ENCODE_EOF) {
			break;
		}
	}
	TRACE_ERR(MP4W_Close(octx), "MP4 close error")

	free(outputBuf);
	free(pcmBufBytes);

	return 0;
}
