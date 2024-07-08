#!/bin/sh
while [ $# -gt 0 ]; do
  cp $1 $1".bak"
  shift
done

