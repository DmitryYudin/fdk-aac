cmake_minimum_required(VERSION 3.14)

project(ffmpeg)

if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug/Release only" FORCE)
endif()

set(FFMPEG_DIR ../../ffmpeg)
if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/${FFMPEG_DIR})
	message(FATAL_ERROR "Can't find ffmpeg project in a '${FFMPEG_DIR}' directory.")
endif()

set(FFMPEG_DIFINES
	HAVE_AV_CONFIG_H
	_ISOC99_SOURCE
	_FILE_OFFSET_BITS=64
	_LARGEFILE_SOURCE
	_USE_MATH_DEFINES
	_CRT_SECURE_NO_WARNINGS
	_CRT_NONSTDC_NO_WARNINGS
)
set(FFMPEG_WARN
	-W3 -wd4018 -wd4146 -wd4244 -wd4305 -wd4554
	# suppress more warnings since we're no going to modify ffmpeg sources
	-wd4028 -wd4090 -wd4101 -wd4102 -wd4133 -wd4267 -wd4334 -wd4996
)
set(FFMPEG_INCLUDE
	${FFMPEG_DIR}
	${FFMPEG_DIR}/compat/atomics/win32
)
set(LIBAVFORMAT_SRC
    libavformat/aacdec.c
    libavformat/allformats.c
    libavformat/apetag.c
    libavformat/av1.c
    libavformat/avc.c
    libavformat/avio.c
    libavformat/aviobuf.c
    libavformat/cutils.c
    libavformat/data_uri.c
    libavformat/dump.c
    libavformat/file.c
    libavformat/file_open.c
    libavformat/format.c
    libavformat/hevc.c
    libavformat/id3v1.c
    libavformat/id3v2.c
    libavformat/img2.c
    libavformat/isom.c
    libavformat/latmenc.c
    libavformat/loasdec.c
    libavformat/metadata.c
    libavformat/mov.c
    libavformat/mov_chan.c
    libavformat/mov_esds.c
    libavformat/movenc.c
	libavformat/movenccenc.c
    libavformat/movenchint.c
    libavformat/mpegts.c
    libavformat/mux.c
    libavformat/network.c
    libavformat/nullenc.c
    libavformat/options.c
    libavformat/os_support.c
    libavformat/protocols.c
    libavformat/qtpalette.c
    libavformat/rawdec.c
    libavformat/rawenc.c
    libavformat/rawutils.c
    libavformat/replaygain.c
    libavformat/riff.c
    libavformat/riffdec.c
    libavformat/riffenc.c
    libavformat/rtp.c
    libavformat/rtpenc_chain.c
    libavformat/sdp.c
    libavformat/url.c
    libavformat/utils.c
    libavformat/vpcc.c
)
set(LIBAVCODEC_SRC
    libavcodec/aac_ac3_parser.c
    libavcodec/aac_adtstoasc_bsf.c
    libavcodec/aac_parser.c
    libavcodec/ac3_parser.c
    libavcodec/ac3tab.c
    libavcodec/adts_header.c
    libavcodec/adts_parser.c
    libavcodec/allcodecs.c
    libavcodec/avdct.c
    libavcodec/avpacket.c
    libavcodec/avpicture.c
    libavcodec/bitstream.c
    libavcodec/bitstream_filter.c
    libavcodec/bitstream_filters.c
    libavcodec/bsf.c
    libavcodec/codec_desc.c
    libavcodec/d3d11va.c
    libavcodec/decode.c
    libavcodec/dirac.c
    libavcodec/dv_profile.c
    libavcodec/encode.c
    libavcodec/faandct.c
    libavcodec/faanidct.c
    libavcodec/fdctdsp.c
    libavcodec/file_open.c
    libavcodec/idctdsp.c
    libavcodec/imgconvert.c
    libavcodec/jfdctfst.c
    libavcodec/jfdctint.c
    libavcodec/jni.c
    libavcodec/jrevdct.c
    libavcodec/latm_parser.c
    libavcodec/mathtables.c
    libavcodec/mediacodec.c
    libavcodec/mjpegenc_huffman.c
    libavcodec/mpeg12framerate.c
    libavcodec/mpeg4audio.c
    libavcodec/mpegaudio_parser.c
    libavcodec/mpegaudiodata.c
    libavcodec/mpegaudiodecheader.c
    libavcodec/null_bsf.c
    libavcodec/options.c
    libavcodec/parser.c
    libavcodec/parsers.c
    libavcodec/profiles.c
    libavcodec/pthread.c
    libavcodec/pthread_frame.c
    libavcodec/pthread_slice.c
    libavcodec/qsv_api.c
    libavcodec/raw.c
    libavcodec/simple_idct.c
    libavcodec/utils.c
	libavcodec/vorbis_parser.c
	libavcodec/xiph.c
)
set(LIBAVUTIL_SRC
    libavutil/adler32.c
    libavutil/aes.c
    libavutil/aes_ctr.c
    libavutil/audio_fifo.c
    libavutil/avsscanf.c
    libavutil/avstring.c
    libavutil/base64.c
    libavutil/blowfish.c
    libavutil/bprint.c
    libavutil/buffer.c
    libavutil/camellia.c
    libavutil/cast5.c
    libavutil/channel_layout.c
    libavutil/color_utils.c
    libavutil/cpu.c
    libavutil/crc.c
    libavutil/des.c
    libavutil/dict.c
    libavutil/display.c
    libavutil/downmix_info.c
    libavutil/encryption_info.c
	libavutil/error.c
	libavutil/eval.c
    libavutil/fifo.c
    libavutil/file.c
    libavutil/file_open.c
    libavutil/fixed_dsp.c
    libavutil/float_dsp.c
    libavutil/frame.c
    libavutil/hash.c
    libavutil/hdr_dynamic_metadata.c
    libavutil/hmac.c
    libavutil/hwcontext.c
    libavutil/imgutils.c
    libavutil/integer.c
    libavutil/intmath.c
    libavutil/lfg.c
    libavutil/lls.c
    libavutil/log.c
    libavutil/log2_tab.c
    libavutil/mastering_display_metadata.c
    libavutil/mathematics.c
    libavutil/md5.c
    libavutil/mem.c
	libavutil/murmur3.c
	libavutil/opt.c
    libavutil/parseutils.c
    libavutil/pixdesc.c
    libavutil/pixelutils.c
    libavutil/random_seed.c
    libavutil/rational.c
    libavutil/rc4.c
    libavutil/reverse.c
    libavutil/ripemd.c
    libavutil/samplefmt.c
    libavutil/sha.c
    libavutil/sha512.c
    libavutil/slicethread.c
    libavutil/spherical.c
    libavutil/stereo3d.c
    libavutil/tea.c
    libavutil/threadmessage.c
    libavutil/time.c
    libavutil/timecode.c
    libavutil/tree.c
    libavutil/twofish.c
    libavutil/tx.c
	libavutil/utils.c
	libavutil/xga_font_data.c
	libavutil/xtea.c
)
list(TRANSFORM LIBAVFORMAT_SRC PREPEND "${FFMPEG_DIR}/")
list(TRANSFORM LIBAVCODEC_SRC PREPEND "${FFMPEG_DIR}/")
list(TRANSFORM LIBAVUTIL_SRC PREPEND "${FFMPEG_DIR}/")

add_library(libavformat STATIC)
add_library(libavcodec STATIC)
add_library(libavutil STATIC)

target_sources(libavformat PRIVATE ${LIBAVFORMAT_SRC})
target_sources(libavcodec PRIVATE ${LIBAVCODEC_SRC})
target_sources(libavutil PRIVATE ${LIBAVUTIL_SRC} ffmpeg_stubs.c)

target_include_directories(libavformat PRIVATE ${FFMPEG_INCLUDE} ./autogen)
target_include_directories(libavcodec PRIVATE ${FFMPEG_INCLUDE} ./autogen)
target_include_directories(libavutil PRIVATE ${FFMPEG_INCLUDE} ./autogen)

target_compile_definitions(libavformat PRIVATE ${FFMPEG_DIFINES} BUILDING_avformat)
target_compile_definitions(libavcodec PRIVATE ${FFMPEG_DIFINES} BUILDING_avcodec)
target_compile_definitions(libavutil PRIVATE ${FFMPEG_DIFINES} BUILDING_avutil)

target_compile_options(libavformat PRIVATE ${FFMPEG_WARN})
target_compile_options(libavcodec PRIVATE ${FFMPEG_WARN})
target_compile_options(libavutil PRIVATE ${FFMPEG_WARN})

target_include_directories(libavutil INTERFACE ${FFMPEG_DIR} ./autogen)
target_link_libraries(libavutil LINK_INTERFACE_LIBRARIES ws2_32 bcrypt)
