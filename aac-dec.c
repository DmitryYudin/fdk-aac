/* ------------------------------------------------------------------
 * Copyright (C) 2013 Martin Storsjo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include "libAACdec/include/aacdecoder_lib.h"
#include "wavwriter.h"

static void usage()
{ 
    printf("aac-dec [-f format] [-h] input.mp4 output.wav \n" 
        "Options:\n"
        " -h, --help                    Print this help message\n"
        " -f, --transport-format <n>    Transport format\n"
        "                                 0: RAW (default, muxed into M4A)\n"
        "                                 1: ADIF\n"
        "                                 2: ADTS\n"
        "                                 6: LATM MCP=1\n"
        "                                 7: LATM MCP=0\n"
        "                     default -> 10: LOAS/LATM (LATM within LOAS)\n"
        " -d                            Dummy flag. For compatibility with aac_mrc\n"
        " -o <output.wav>               Output file. For compatibility with aac_mrc\n"
    );
}

int main(int argc, char *argv[]) {
	const char *infile = NULL, *outfile = NULL;
	FILE *in = NULL;
	void *wav = NULL;
	int output_size;
	uint8_t *output_buf = NULL;
	int16_t *decode_buf = NULL;
	HANDLE_AACDECODER handle = NULL;
	int frame_size = 0;
	int status = 0;
	unsigned samples_decoded = 0;
	if (argc < 3) {
		usage();
		return 1;
	}

	static const struct option long_options[] = {
		{"help", no_argument, 0, 'h'},
		{"transport-format", required_argument, 0, 'f'},
		{0, 0, 0, 0},
	};
	int ch, n;
	TRANSPORT_TYPE transport_format = TT_UNKNOWN;
	while ((ch = getopt_long(argc, argv, "hf:o:", long_options, 0)) != EOF) {
		switch (ch) {
		case 'h':
			return usage(), -1;
		default:
			return usage(), -1;
		case 'f': // currently, only ATDS and LOAS are compatible with decoder (the reason is unknown)
			if (sscanf(optarg, "%u", &n) != 1) {
				fprintf(stderr, "invalid arg for transport-format\n");
				return -1;
			}
			transport_format = (TRANSPORT_TYPE)n;
			break;
		case 'o':
			outfile = optarg;
			break;
		}
	}
	while(argv[optind] != NULL) { // rest of options
		if (infile == NULL) {
			infile = argv[optind++];
			continue;
		}
		if (outfile == NULL) {
			outfile = argv[optind++];
			continue;
		}
		fprintf(stderr, "Error: Too many options\n");
	}
	if (infile == NULL) {
		fprintf(stderr, "Error: input file name not set\n");
		return 1;
	}
	if (outfile == NULL) {
		fprintf(stderr, "Error: output file name not set\n");
		return 1;
	}
	if (transport_format == TT_UNKNOWN) {
		transport_format = TT_MP4_LOAS;
	}

	handle = aacDecoder_Open(transport_format, 1);
	in = fopen(infile, "rb");
	if (!in) {
		perror(infile);
		return 1;
	}

	output_size = 8*2*2048;
	output_buf = (uint8_t*) malloc(output_size);
	decode_buf = (int16_t*) malloc(output_size);

	while (1) {
		uint8_t packet[32768/2], *ptr = packet;
		int n, i;
		UINT valid, packet_size;
		AAC_DECODER_ERROR err;
		n = (int)fread(packet, 1, sizeof(packet), in);
		if (n <= 0) {
			if(feof(in)) {
				goto end;
			}
			fprintf(stderr, "Error: Failed reading input file: '%s'\n", infile);
			status = 1;
			goto end;
		}
		packet_size = n;
		valid = packet_size;
		err = aacDecoder_Fill(handle, &ptr, &packet_size, &valid);
		if (err != AAC_DEC_OK) {
			fprintf(stderr, "Fill failed: %x\n", err);
			status = 1;
			goto end;
		}
		if (valid != 0) {
			fprintf(stderr, "Unable to feed all %d input bytes, %d bytes left\n", n, valid);
			status = 1;
			goto end;
		}
		while (1) {
			err = aacDecoder_DecodeFrame(handle, decode_buf, output_size / sizeof(INT_PCM), 0);
			if (err == AAC_DEC_NOT_ENOUGH_BITS)
				break;
			if (err == AAC_DEC_TRANSPORT_SYNC_ERROR)
				break;
			if (err != AAC_DEC_OK) {
				fprintf(stderr, "Decode failed: 0x%x\n", err);
				status = 1;
				goto end;
			}
			if (!wav) {
				CStreamInfo *info = aacDecoder_GetStreamInfo(handle);
				if (!info || info->sampleRate <= 0) {
					fprintf(stderr, "No stream info\n");
					status = 1;
					goto end;
				}
				frame_size = info->frameSize * info->numChannels;
				// Note, this probably doesn't return channels > 2 in the right order for wav
				wav = wav_write_open(outfile, info->sampleRate, 16, info->numChannels);
				if (!wav) {
					fprintf(stderr, "Error: Failed writing output file: '%s'\n", outfile);
					status = 1;
					goto end;
				}
				samples_decoded += frame_size;
			}
			for (i = 0; i < frame_size; i++) {
				uint8_t* out = &output_buf[2*i];
				out[0] = decode_buf[i] & 0xff;
				out[1] = decode_buf[i] >> 8;
			}
			wav_write_data(wav, output_buf, 2*frame_size);
		}
	}
end:
	if(samples_decoded == 0 && status == 0) {
		fprintf(stderr, "Error: No samples decoded. Perhaps wrong input format.\n");
		status = 1;
	}
	if(output_buf)
		free(output_buf);
	if (decode_buf)
		free(decode_buf);
	if (in)
		fclose(in);
	if (wav)
		wav_write_close(wav);
	if(handle)
		aacDecoder_Close(handle);
	return status;
}

