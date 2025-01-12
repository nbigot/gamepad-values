mkdir build
# configure
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S "." -B "./build" -G "Visual Studio 15 2017" -T host=x86 -A x64
# build
cmake --build ./build
