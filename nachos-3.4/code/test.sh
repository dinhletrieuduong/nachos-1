#!/bin/sh

RUN_CMD='./userprog/nachos -rs 1023 -x'

function runcmd()
{
    echo "[------ Test $1 ------]"
    sh -c "./userprog/nachos -rs 1023 -x ./test/$1"
    echo ""
    echo ""
}

runcmd "halt"
runcmd "createfile"
runcmd "test"
runcmd "args"
runcmd "shell"
