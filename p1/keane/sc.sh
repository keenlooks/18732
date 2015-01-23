#!/usr/local/bin/bash

echo ""
objdump -d -j .data loader | grep -v "\.\.\." | grep -v 'file' | \
cut -d: -f2 | cut -f1-7 -d' ' | tr -s ' ' | tr '\t' ' ' | \
sed 's/ $//g' | sed 's/ /\\x/g' | tr -d '\n' | sed -e 's/.\{60\}/&:/g' | \
tr ":" "\n" | sed 's/^/"/' | sed 's/$/"/g'
echo -e ""

