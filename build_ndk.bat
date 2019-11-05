@echo off

::set V=1
::set NDK_LOG=1
::set NDK_DEBUG=1

set NDK_OUT=.build_ndk
set NDK_LIBS_OUT=%NDK_OUT%/bin
set NCPU=1

if "%NUMBER_OF_PROCESSORS%" gtr "1" set /A NCPU=%NUMBER_OF_PROCESSORS%-1
call ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk -j %NCPU% %ENV_APP_ABI% %*
