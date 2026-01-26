#!/usr/bin/env bash
echo "Build Eco.System1"
DIR=$(cd "$(dirname "$0")"; pwd)
echo $DIR
export TARGET=0
export DEBUG=0
make clean -f Makefile
make -f Makefile
make clean -f MakefileExe
make -f MakefileExe
export TARGET=0
export DEBUG=1
make clean -f Makefile
make -f Makefile
make clean -f MakefileExe
make -f MakefileExe
export TARGET=1
export DEBUG=0
make clean -f Makefile
make -f Makefile
export TARGET=1
export DEBUG=1
make clean -f Makefile
make -f Makefile
make clean -f MakefileExe
make -f MakefileExe

mkdir -p $ECO_FRAMEWORK/Eco.System1/BuildFiles/Linux/arm64-v8a/StaticRelease && cp ../../../BuildFiles/Linux/arm64-v8a/StaticRelease/lib00000000000000000000000053595333.a $ECO_FRAMEWORK/Eco.System1/BuildFiles/Linux/arm64-v8a/StaticRelease
mkdir -p $ECO_FRAMEWORK/Eco.System1/BuildFiles/Linux/arm64-v8a/DynamicRelease && cp ../../../BuildFiles/Linux/arm64-v8a/DynamicRelease/lib00000000000000000000000053595333.a $ECO_FRAMEWORK/Eco.System1/BuildFiles/Linux/arm64-v8a/DynamicRelease

