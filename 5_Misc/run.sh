#!/bin/bash
## MACOS
qemu-system-x86_64 -hda 1_Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=speaker -machine pcspk-audiodev=speaker
