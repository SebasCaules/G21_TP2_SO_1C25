#!/bin/bash

CONTAINER_NAME="SO2"

docker start $CONTAINER_NAME

docker exec -u root -it $CONTAINER_NAME make clean    -C /root/2_Toolchain
docker exec -u root -it $CONTAINER_NAME make all      -C /root/2_Toolchain
docker exec -u root -it $CONTAINER_NAME make clean    -C /root/
docker exec -u root -it $CONTAINER_NAME pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
docker exec -u root -it $CONTAINER_NAME make check    -C /root/