Remove-Item -Recurse -Path build

mkdir build

Set-Location  build

cmake .. -G "MinGW Makefiles"

mingw32-make

Set-Location ..

.\build\femora.exe

