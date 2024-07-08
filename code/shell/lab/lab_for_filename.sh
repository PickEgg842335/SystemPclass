#!/bin/sh
files=`ls`
for f in $files
do
   echo "processing $file"
   mv $f `echo $f | tr '[A-Z]' '[a-z]'`
done

