#!/bin/bash
FILE1="testFile1.txt"
FILE2="testFile2.txt"
opt=1
if [ $opt -eq 1 ]
then
	cat $FILE1
else
	cat $FILE2
fi
