#!/bin/sh
files=`ls | grep *.htm `
for i in $files
do
  echo "processing $i"
  cp $i $i"l"
done

