cmake_minimum_required(VERSION 3.14)

project(fdk-aac)

if(CMAKE_CONFIGURATION_TYPES) # https://stackoverflow.com/questions/31661264/cmake-generators-for-visual-studio-do-not-set-cmake-configuration-types
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug/Release only" FORCE)
endif()

#
# library
#
add_library(fdk-aac STATIC)

set(AACDEC_SRC
    libAACdec/src/FDK_delay.cpp
    libAACdec/src/aac_ram.cpp
    libAACdec/src/aac_rom.cpp
    libAACdec/src/aacdec_drc.cpp
    libAACdec/src/aacdec_hcr.cpp
    libAACdec/src/aacdec_hcr_bit.cpp
    libAACdec/src/aacdec_hcrs.cpp
    libAACdec/src/aacdec_pns.cpp
    libAACdec/src/aacdec_tns.cpp
    libAACdec/src/aacdecoder.cpp
    libAACdec/src/aacdecoder_lib.cpp
    libAACdec/src/block.cpp
    libAACdec/src/channel.cpp
    libAACdec/src/channelinfo.cpp
    libAACdec/src/conceal.cpp
    libAACdec/src/ldfiltbank.cpp
    libAACdec/src/pulsedata.cpp
    libAACdec/src/rvlc.cpp
    libAACdec/src/rvlcbit.cpp
    libAACdec/src/rvlcconceal.cpp
    libAACdec/src/stereo.cpp
    libAACdec/src/usacdec_ace_d4t64.cpp
    libAACdec/src/usacdec_ace_ltp.cpp
    libAACdec/src/usacdec_acelp.cpp
    libAACdec/src/usacdec_fac.cpp
    libAACdec/src/usacdec_lpc.cpp
    libAACdec/src/usacdec_lpd.cpp
    libAACdec/src/usacdec_rom.cpp
)
set(AACENC_SRC
    libAACenc/src/aacEnc_ram.cpp
    libAACenc/src/aacEnc_rom.cpp
    libAACenc/src/aacenc.cpp
    libAACenc/src/aacenc_lib.cpp
    libAACenc/src/aacenc_pns.cpp
    libAACenc/src/aacenc_tns.cpp
    libAACenc/src/adj_thr.cpp
    libAACenc/src/band_nrg.cpp
    libAACenc/src/bandwidth.cpp
    libAACenc/src/bit_cnt.cpp
    libAACenc/src/bitenc.cpp
    libAACenc/src/block_switch.cpp
    libAACenc/src/channel_map.cpp
    libAACenc/src/chaosmeasure.cpp
    libAACenc/src/dyn_bits.cpp
    libAACenc/src/grp_data.cpp
    libAACenc/src/intensity.cpp
    libAACenc/src/line_pe.cpp
    libAACenc/src/metadata_compressor.cpp
    libAACenc/src/metadata_main.cpp
    libAACenc/src/mps_main.cpp
    libAACenc/src/ms_stereo.cpp
    libAACenc/src/noisedet.cpp
    libAACenc/src/pnsparam.cpp
    libAACenc/src/pre_echo_control.cpp
    libAACenc/src/psy_configuration.cpp
    libAACenc/src/psy_main.cpp
    libAACenc/src/qc_main.cpp
    libAACenc/src/quantize.cpp
    libAACenc/src/sf_estim.cpp
    libAACenc/src/spreading.cpp
    libAACenc/src/tonality.cpp
    libAACenc/src/transform.cpp
)
set(ARITHCODING_SRC
    libArithCoding/src/ac_arith_coder.cpp
)
set(DRCDEC_SRC
    libDRCdec/src/FDK_drcDecLib.cpp
    libDRCdec/src/drcDec_gainDecoder.cpp
    libDRCdec/src/drcDec_reader.cpp
    libDRCdec/src/drcDec_rom.cpp
    libDRCdec/src/drcDec_selectionProcess.cpp
    libDRCdec/src/drcDec_tools.cpp
    libDRCdec/src/drcGainDec_init.cpp
    libDRCdec/src/drcGainDec_preprocess.cpp
    libDRCdec/src/drcGainDec_process.cpp
)
set(FDK_SRC
    libFDK/src/FDK_bitbuffer.cpp
    libFDK/src/FDK_core.cpp
    libFDK/src/FDK_crc.cpp
    libFDK/src/FDK_decorrelate.cpp
    libFDK/src/FDK_hybrid.cpp
    libFDK/src/FDK_lpc.cpp
    libFDK/src/FDK_matrixCalloc.cpp
    libFDK/src/FDK_qmf_domain.cpp
    libFDK/src/FDK_tools_rom.cpp
    libFDK/src/FDK_trigFcts.cpp
    libFDK/src/autocorr2nd.cpp
    libFDK/src/dct.cpp
    libFDK/src/fft.cpp
    libFDK/src/fft_rad2.cpp
    libFDK/src/fixpoint_math.cpp
    libFDK/src/huff_nodes.cpp
    libFDK/src/mdct.cpp
    libFDK/src/nlc_dec.cpp
    libFDK/src/qmf.cpp
    libFDK/src/scale.cpp
)
set(MPEGTPDEC_SRC
    libMpegTPDec/src/tpdec_adif.cpp
    libMpegTPDec/src/tpdec_adts.cpp
    libMpegTPDec/src/tpdec_asc.cpp
    libMpegTPDec/src/tpdec_drm.cpp
    libMpegTPDec/src/tpdec_latm.cpp
    libMpegTPDec/src/tpdec_lib.cpp
)
set(MPEGTPENC_SRC
    libMpegTPEnc/src/tpenc_adif.cpp
    libMpegTPEnc/src/tpenc_adts.cpp
    libMpegTPEnc/src/tpenc_asc.cpp
    libMpegTPEnc/src/tpenc_latm.cpp
    libMpegTPEnc/src/tpenc_lib.cpp
)
set(PCMUTILS_SRC
    libPCMutils/src/limiter.cpp
    libPCMutils/src/pcm_utils.cpp
    libPCMutils/src/pcmdmx_lib.cpp
)
set(SACDEC_SRC
    libSACdec/src/sac_bitdec.cpp
    libSACdec/src/sac_calcM1andM2.cpp
    libSACdec/src/sac_dec.cpp
    libSACdec/src/sac_dec_conceal.cpp
    libSACdec/src/sac_dec_lib.cpp
    libSACdec/src/sac_process.cpp
    libSACdec/src/sac_qmf.cpp
    libSACdec/src/sac_reshapeBBEnv.cpp
    libSACdec/src/sac_rom.cpp
    libSACdec/src/sac_smoothing.cpp
    libSACdec/src/sac_stp.cpp
    libSACdec/src/sac_tsd.cpp
)
set(SACENC_SRC
    libSACenc/src/sacenc_bitstream.cpp
    libSACenc/src/sacenc_delay.cpp
    libSACenc/src/sacenc_dmx_tdom_enh.cpp
    libSACenc/src/sacenc_filter.cpp
    libSACenc/src/sacenc_framewindowing.cpp
    libSACenc/src/sacenc_huff_tab.cpp
    libSACenc/src/sacenc_lib.cpp
    libSACenc/src/sacenc_nlc_enc.cpp
    libSACenc/src/sacenc_onsetdetect.cpp
    libSACenc/src/sacenc_paramextract.cpp
    libSACenc/src/sacenc_staticgain.cpp
    libSACenc/src/sacenc_tree.cpp
    libSACenc/src/sacenc_vectorfunctions.cpp
)
set(SBRDEC_SRC
    libSBRdec/src/HFgen_preFlat.cpp
    libSBRdec/src/env_calc.cpp
    libSBRdec/src/env_dec.cpp
    libSBRdec/src/env_extr.cpp
    libSBRdec/src/hbe.cpp
    libSBRdec/src/huff_dec.cpp
    libSBRdec/src/lpp_tran.cpp
    libSBRdec/src/psbitdec.cpp
    libSBRdec/src/psdec.cpp
    libSBRdec/src/psdec_drm.cpp
    libSBRdec/src/psdecrom_drm.cpp
    libSBRdec/src/pvc_dec.cpp
    libSBRdec/src/sbr_crc.cpp
    libSBRdec/src/sbr_deb.cpp
    libSBRdec/src/sbr_dec.cpp
    libSBRdec/src/sbr_ram.cpp
    libSBRdec/src/sbr_rom.cpp
    libSBRdec/src/sbrdec_drc.cpp
    libSBRdec/src/sbrdec_freq_sca.cpp
    libSBRdec/src/sbrdecoder.cpp
)
set(SBRENC_SRC
    libSBRenc/src/bit_sbr.cpp
    libSBRenc/src/code_env.cpp
    libSBRenc/src/env_bit.cpp
    libSBRenc/src/env_est.cpp
    libSBRenc/src/fram_gen.cpp
    libSBRenc/src/invf_est.cpp
    libSBRenc/src/mh_det.cpp
    libSBRenc/src/nf_est.cpp
    libSBRenc/src/ps_bitenc.cpp
    libSBRenc/src/ps_encode.cpp
    libSBRenc/src/ps_main.cpp
    libSBRenc/src/resampler.cpp
    libSBRenc/src/sbr_encoder.cpp
    libSBRenc/src/sbr_misc.cpp
    libSBRenc/src/sbrenc_freq_sca.cpp
    libSBRenc/src/sbrenc_ram.cpp
    libSBRenc/src/sbrenc_rom.cpp
    libSBRenc/src/ton_corr.cpp
    libSBRenc/src/tran_det.cpp
)
set(SYS_SRC
    libSYS/src/genericStds.cpp
    libSYS/src/syslib_channelMapDescr.cpp
)
set(libfdk_aac_SOURCES
    ${AACDEC_SRC} ${AACENC_SRC}
    ${ARITHCODING_SRC}
    ${DRCDEC_SRC}
    ${MPEGTPDEC_SRC} ${MPEGTPENC_SRC}
    ${SACDEC_SRC} ${SACENC_SRC}
    ${SBRDEC_SRC} ${SBRENC_SRC}
    ${PCMUTILS_SRC} ${FDK_SRC} ${SYS_SRC}
)
target_sources(fdk-aac PRIVATE
	${libfdk_aac_SOURCES}
)
target_include_directories(fdk-aac PRIVATE
    libAACdec/include
    libAACenc/include
    libArithCoding/include
    libDRCdec/include
    libSACdec/include
    libSACenc/include
    libSBRdec/include
    libSBRenc/include
    libMpegTPDec/include
    libMpegTPEnc/include
    libSYS/include
    libFDK/include
    libPCMutils/include
)
target_include_directories(fdk-aac PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libAACenc/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libAACdec/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libSYS/include>
)

#
# Tests
#
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	add_subdirectory(ffmpeg)
	message(STATUS "FFmpeg preconfigured files reside locally in '${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg' folder")

    add_executable(aac-enc)
    add_executable(aac-dec)
    add_executable(test-encode-decode)
    add_executable(m4a-dec)
    add_executable(m4a-enc)

    set(aac_enc_SOURCES aac-enc.c wavreader.c)
    set(aac_dec_SOURCES aac-dec.c wavwriter.c)
    set(test_encode_decode_SOURCES test-encode-decode.c wavreader.c)
    set(m4a_dec_SOURCES m4a-dec wavwriter.c)
    set(m4a_enc_SOURCES m4a-enc wavreader.c)

    target_sources(aac-enc PRIVATE ${aac_enc_SOURCES})
    target_sources(aac-dec PRIVATE ${aac_dec_SOURCES})
    target_sources(test-encode-decode PRIVATE ${test_encode_decode_SOURCES})
    target_sources(m4a-dec PRIVATE ${m4a_dec_SOURCES})
    target_sources(m4a-enc PRIVATE ${m4a_enc_SOURCES})

    target_include_directories(aac-enc PRIVATE win32)
    target_include_directories(aac-dec PRIVATE win32)
    target_include_directories(test-encode-decode PRIVATE win32)
    target_include_directories(m4a-dec PRIVATE win32)
    target_include_directories(m4a-enc PRIVATE win32)

    target_compile_definitions(aac-enc PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(aac-dec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(test-encode-decode PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(m4a-dec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(m4a-enc PRIVATE _CRT_SECURE_NO_WARNINGS)

    target_link_libraries(aac-enc fdk-aac)
    target_link_libraries(aac-dec fdk-aac)
    target_link_libraries(test-encode-decode fdk-aac)
    target_link_libraries(m4a-dec fdk-aac libavformat libavutil libavcodec)
    target_link_libraries(m4a-enc fdk-aac libavformat libavutil libavcodec)
endif()