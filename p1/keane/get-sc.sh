#!/usr/local/bin/bash

objdump -d ./${1} | grep -v 'file' |cut -d: -f2|cut -f1-7 -d' '|tr -s ' '| \
tr '\t' ' '|sed 's/ $//g'|sed 's/ /\\x/g'|tr -d '\n'|sed 's/^/"/'|sed 's/$/"/g'

