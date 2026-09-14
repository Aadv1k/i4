#!/bin/bash

CC="gcc"
CFLAGS="-Wall -Wextra -pedantic -std=c23"

if [ -n "$DEBUG" ]; then
    CFLAGS="${CFLAGS} -O0 -g -ggdb"
fi

SRC_DIR=src
TEST_DIR=tests
THIRDPARTY_DIR=thirdparty
OUTPUT_DIR=dist

SRC_FILES=($SRC_DIR/*.c)
TEST_FILES=($TEST_DIR/*.c)
THIRDPARTY_FILES=($THIRDPARTY_DIR/munit/munit.c)


ensure_output_dir() {
    mkdir -p dist/
}

build_test() {
    ensure_output_dir

    $CC $CFLAGS -I$THIRDPARTY_DIR -o $OUTPUT_DIR/i4_test ${SRC_FILES[@]} ${TEST_FILES[@]} ${THIRDPARTY_FILES[@]}

}

build_test
