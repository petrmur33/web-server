#!/bin/sh

CC="tcc64"
CFLAGS="-c"
LIBS_PATH="-Lc:\\windows\\system32"
LIBS="-lws2_32"
SRC_PATH="./src"
OUTPUT="a.exe"

$CC $CFLAGS -o ./obj/tcp_socket.o       $SRC_PATH/tcp_socket.c 
$CC $CFLAGS -o ./obj/webpages.o         $SRC_PATH/webpages.c
$CC $CFLAGS -o ./obj/webpage.o          $SRC_PATH/webpage.c
$CC $CFLAGS -o ./obj/request.o          $SRC_PATH/request.c
$CC $CFLAGS -o ./obj/server.o           $SRC_PATH/server.c
$CC $CFLAGS -o ./obj/main.o             $SRC_PATH/main.c
$CC $CFLAGS -o ./obj/utils.o            $SRC_PATH/utils.c
$CC $CFLAGS -o ./obj/parser.o           $SRC_PATH/parser.c
$CC $CFLAGS -o ./obj/handler_factory.o  $SRC_PATH/handler_factory.c
$CC $CFLAGS -o ./obj/multithreading.o   $SRC_PATH/multithreading.c
$CC $CFLAGS -o ./obj/logger.o           $SRC_PATH/logger.c

$CC -o $OUTPUT $LIBS_PATH $LIBS \
    ./obj/tcp_socket.o \
    ./obj/webpages.o \
    ./obj/webpage.o \
    ./obj/request.o \
    ./obj/server.o \
    ./obj/main.o \
    ./obj/parser.o \
    ./obj/handler_factory.o \
    ./obj/multithreading.o \
    ./obj/logger.o \
    ./obj/utils.o
