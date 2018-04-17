#!/bin/bash
set -e
set -v

RUN_LOG="$PWD/run.log"
GDB_BT_SCRIPT=`dirname "$PWD/$0"`/gdb_bt_script

exec &> >(tee "${RUN_LOG}")

mkdir -p build
pushd build > /dev/null

cmake ..
make

gdb -n -batch -x "${GDB_BT_SCRIPT}" --args ./snu_graphics
popd > /dev/null
