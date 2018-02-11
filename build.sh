#!/bin/bash

rm ./qt_bck_rn1

qmake ./qt_bck_rn1.pro -spec linux-g++ 
make

rm ./*.o
rm ./Makefile

