#!/bin/bash

#compilation des fichiers sources, creation de la librairie et copiage dans les bon dossiers 
clear

expectedArguments=2

if [ $# -ne $expectedArguments ]
then
    echo "usage : ./makeLib dossier nameLib"
    exit 1
fi

dossierLib=$1
nameLiba=$2".a"
nameLibh=$2".h"
cph=/usr/include
cpa=/usr/lib

cd $dossierLib

if [ -f *.a ]
then 
    rm *.a
fi

fc=$(ls *.c)

gcc -c $(ls *.c)
ar r $nameLiba $(ls *.o)
ranlib $nameLiba
sudo mv $nameLiba $cpa
if [ -f $nameLibh ]
then
    sudo mv $nameLibh $cph
fi

rm $(ls *.o)

