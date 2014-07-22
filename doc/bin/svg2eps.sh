#!/bin/bash

for file in *.svg
do
	echo $file

	inkscape -f $file -E $(echo "$file" | cut -d'.' --complement -f2-).eps 
done

