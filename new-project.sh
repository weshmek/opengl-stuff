#!/bin/sh

mkdir ${1}
cp shared/template.txt ${1}/${1}.c
cp shared/Makefile_template ${1}/Makefile
sed -i "s/template/${1}/" ${1}/Makefile
