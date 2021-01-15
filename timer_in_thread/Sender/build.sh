#!/bin/bash

g++ ../include/Thread.cpp MsgSender.cpp main.cpp -I./ -I../include/ -L./ -L../include/ -lpthread -lrt -std=c++11 -o SENDER