#!bin/bash 

rootDir=$(dirname $( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd ))
echo $rootDir

if [ "$1" == "clean" ]
  then
    rm -r ${rootDir}/build ${rootDir}/bin/demo
    exit
fi

mkdir ${rootDir}/build
cd ${rootDir}/build
cmake ..
cmake --build .
mv demo ${rootDir}/bin/

