#!/bin/bash

od -tf -N79999999 < /dev/urandom |
cut -c 9- |
tr ' ' '\n' |
tr -s '\n' |
head -n10000000 - > file.txt
