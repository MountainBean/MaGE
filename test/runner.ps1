cmake -B build -S . &&
cmake --build ./build && 
./build/Debug/tests.exe $args
