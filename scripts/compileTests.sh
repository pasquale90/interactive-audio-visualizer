#!bin/bash 

rootDir=$(dirname $( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd ))

exeFile=test_all
testDir=test

if [ "$1" == "clean" ]
  then
    rm -r ${testDir}/build ${rootDir}/bin/${exeFile}
    exit
fi

mkdir ${rootDir}/${testDir}/build
cd ${rootDir}/${testDir}/build
cmake ..
cmake --build .
mv ${exeFile} ${rootDir}/bin/
