@echo off
set BUILD_DIR=.build
if not exist %BUILD_DIR% (mkdir %BUILD_DIR% 2> nul)
if not "%ENV_FORCE_X32_BUILD%" == "" (set OPT=-A win32)
cmake.exe %OPT% -B %BUILD_DIR% -S . || exit /B
echo Successfully generated into '%BUILD_DIR%' folder.
if "%1" == "b" (
	cmake --build %BUILD_DIR% --config Release || exit /B
	echo Successfully built
) else if "%1" == "o" (
	cmake --open %BUILD_DIR% || exit /B
	echo Successfully opened
) else (
	echo Run '%~nx0 b' to build generated solution or '%~nx0 o' to open it with Visual Studio IDE.
)