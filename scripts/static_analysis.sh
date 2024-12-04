#!bin/bash

#cppcheck
mkdir checks
cd checks
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cppcheck --project=compile_commands.json --enable=all
