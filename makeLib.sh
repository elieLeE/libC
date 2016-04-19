#!/bin/bash

#compilation des fichiers sources, creation de la librairie et copiage dans les bon dossiers 
cph=/usr/include
mva=/usr/lib
nameLib=libcmine
nameLiba=$nameLib".a"
nameLibh="../"$nameLib".h"

cd ./src

if [ -f *.a ]
then 
    rm *.a
fi

gcc -c $(ls *.c */*.c)
ar r $nameLiba $(ls *.o)
ranlib $nameLiba
sudo mv $nameLiba $mva
sudo cp $nameLibh $cph

rm $(ls *.o)

