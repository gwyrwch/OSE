#!/bin/bash

argn=$#


if [[ "$argn" -lt "2" ]]; then
	echo Wrong usage
else
	find ~/Documents -maxdepth 1 -name "*.$1" >> $2
fi
