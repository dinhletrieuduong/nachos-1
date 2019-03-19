#!/bin/sh

CUR_DIR=$(pwd)

rm *~

cd $CUR_DIR/test
rm *~
rm *.o
rm *.coff

cd $CUR_DIR/network
rm *~
rm *.o

cd $CUR_DIR/filesys
rm *~
rm *.o

cd $CUR_DIR/threads
rm *~
rm *.o

cd $CUR_DIR/vm
rm *~
rm *.o

cd $CUR_DIR/userprog
rm *~
rm *.o