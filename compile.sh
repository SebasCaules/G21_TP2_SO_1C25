#!/bin/bash

if [ "$1" == "buddy" ]; then
    MM="USE_BUDDY"
else
    MM=""
fi

# Name of the Docker container
CONTAINER_NAME="SO2"

# Start the Docker container
docker start $CONTAINER_NAME

# Verifica si el contenedor está corriendo
RUNNING=$(docker inspect -f '{{.State.Running}}' $CONTAINER_NAME 2>/dev/null)
if [ "$RUNNING" != "true" ]; then
    echo "Error: el contenedor $CONTAINER_NAME no está corriendo."
    exit 1
fi

# Clean and build the project in the specified directories
docker exec -it $CONTAINER_NAME make clean -C /root/2_Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root

# Execute the make commands
docker exec -it $CONTAINER_NAME make -C /root/2_Toolchain MM="$MM"
MAKE_TOOLCHAIN_EXIT_CODE=$?

docker exec -it $CONTAINER_NAME make -C /root MM="$MM"
MAKE_ROOT_EXIT_CODE=$?

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Check if both make commands were successful
if [[ $MAKE_TOOLCHAIN_EXIT_CODE -eq 0 && $MAKE_ROOT_EXIT_CODE -eq 0 ]]; then
    echo -e "${GREEN}Compilation successful!${NC}"
    echo -e "${GREEN}Run './run.sh' to start the kernel${NC}"
else
    echo -e "${RED}Compilation failed!${NC}"
fi
