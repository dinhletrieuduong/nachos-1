#!/bin/sh
echo ""
echo "*** Executing $1 ***"
echo ""
./userprog/nachos -rs 1023 -x $1
