#!/bin/sh

cp -r ${1} ${2} || exit 1
mv ${2}/${1}.c ${2}/${2}.c

sed -i "s/${1}/${2}/" ${2}/Makefile
rm ${2}/${1}.*
