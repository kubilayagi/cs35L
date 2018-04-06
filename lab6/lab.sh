#!/bin/bash

echo "parallel=1" >> log.txt

(time -p sort --parallel=1 -g file.txt) 2>> log.txt


echo "parallel=2\n" >> log.txt

(time -p sort --parallel=2 -g file.txt) 2>> log.txt


echo "parallel=4\n" >> log.txt

(time -p sort --parallel=4 -g file.txt) 2>> log.txt


echo "parallel=8\n" >> log.txt

(time -p sort --parallel=8 -g file.txt) 2>> log.txt


