@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "SRC_DIR=%CI_PROJECT_DIR%"
if not defined SRC_DIR set "SRC_DIR=%cd%"
set "BUILD_DIR=%SRC_DIR%\build-cpack-windows"
set "OUTPUT_DIR=%SRC_DIR%\dist\cpack"
set "MINICONDA_DIR=%BUILD_DIR%\miniconda3"
set "CONDA_ENV=%BUILD_DIR%\wasp_ci"

if exist "%BUILD_DIR%" rmdir /S /Q "%BUILD_DIR%"
if exist "%OUTPUT_DIR%" rmdir /S /Q "%OUTPUT_DIR%"
mkdir "%BUILD_DIR%" || exit /B 1
mkdir "%OUTPUT_DIR%" || exit /B 1

curl --fail --location --retry 3 ^
  https://code-int.ornl.gov/lefebvre/miniconda/-/raw/main/Miniconda3-py310_24.5.0-0-Windows-x86_64.exe ^
  --output "%BUILD_DIR%\miniconda.exe"
if errorlevel 1 exit /B %errorlevel%

"%BUILD_DIR%\miniconda.exe" /S /D=%MINICONDA_DIR%
if errorlevel 1 exit /B %errorlevel%
call "%MINICONDA_DIR%\Scripts\activate.bat"
call conda env create --prefix "%CONDA_ENV%" --file "%SRC_DIR%\ci\env.yml"
if errorlevel 1 exit /B %errorlevel%
call conda activate "%CONDA_ENV%"

set "PATH=C:\Program Files (x86)\NSIS;%PATH%"
where makensis.exe
if errorlevel 1 (
  echo NSIS is required to create the Windows installer.
  exit /B 1
)

cmake -S "%SRC_DIR%" -B "%BUILD_DIR%\package" ^
  -G "Visual Studio 17 2022" -A x64 ^
  -DBUILD_SHARED_LIBS=OFF ^
  -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded ^
  -DCPACK_GENERATOR=NSIS ^
  -DWASP_CPACK_PLATFORM=Windows-x86_64 ^
  -DWASP_ENABLE_SWIG=OFF ^
  -Dwasp_ENABLE_ALL_PACKAGES=ON ^
  -Dwasp_ENABLE_CPACK_PACKAGING=ON ^
  -Dwasp_ENABLE_TESTS=OFF ^
  -Dwasp_ENABLE_testframework=OFF ^
  -Dwasp_ENABLE_wasppy=OFF
if errorlevel 1 exit /B %errorlevel%

cmake --build "%BUILD_DIR%\package" --target package --config Release --parallel
if errorlevel 1 exit /B %errorlevel%

if not exist "%BUILD_DIR%\package\*.exe" (
  echo CPack did not produce an NSIS installer.
  exit /B 1
)
for %%G in ("%BUILD_DIR%\package\*.exe") do (
  copy /Y "%%~fG" "%OUTPUT_DIR%\" >nul
  if errorlevel 1 exit /B !errorlevel!
)
if exist "%OUTPUT_DIR%\SHA256SUMS-Windows-x86_64.txt" del /Q "%OUTPUT_DIR%\SHA256SUMS-Windows-x86_64.txt"
for %%G in ("%OUTPUT_DIR%\*.exe") do (
  cmake -E sha256sum "%%~fG" >> "%OUTPUT_DIR%\SHA256SUMS-Windows-x86_64.txt"
  if errorlevel 1 exit /B !errorlevel!
)

endlocal
