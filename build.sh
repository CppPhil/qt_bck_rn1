#!/bin/bash

rm ./qt_bck_rn1

qmake /home/phil/QtProjects/qt_bck_rn1/qt_bck_rn1.pro -spec linux-g++ 
make

rm ./*.o
rm ./Makefile

