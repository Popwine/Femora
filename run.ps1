Remove-Item -Recurse -Path build

mkdir build

Set-Location  build

cmake .. -G "MinGW Makefiles" -DUSE_DOUBLE_PRECISION=ON

mingw32-make

Set-Location ..

.\build\femora.exe

