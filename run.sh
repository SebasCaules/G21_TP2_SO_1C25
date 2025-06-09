#!/bin/bash

CONTAINER_NAME="SO2"

if [[ "$1" = "gdb" ]]; then
  qemu-system-x86_64 -s -S -hda 1_Image/x64BareBonesImage.qcow2 -m 512
else
  qemu-system-x86_64 -hda 1_Image/x64BareBonesImage.qcow2 -m 512
fi

docker exec -it $CONTAINER_NAME make clean -C /root