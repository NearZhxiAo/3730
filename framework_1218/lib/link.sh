#!/bin/sh
cd ../$1
make $2
cd -
ln -sf ../$1/$2 $2
