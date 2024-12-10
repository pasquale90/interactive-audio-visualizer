#!bin/bash

#cppcheck
mkdir checks
cd checks
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

#cppcheck
cppcheck --project=compile_commands.json --enable=performance,warning,style > cppcheck.log
#clang-tidy
run-clang-tidy -checks='bugprone-*,performance-*,clang-analyzer-*' > clang-tidy.log