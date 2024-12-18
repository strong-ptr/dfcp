cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B build
cmake --build build
cp build/compile_commands.json .
