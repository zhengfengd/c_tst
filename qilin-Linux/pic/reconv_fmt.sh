#!/bin/sh

echo "the following result:"

for FROM in `ls IMG_*.JPG`;
do
	echo "logo.new.$FROM --> revert.$FROM";
	convert -sample 4000 logo.new.$FROM revert.$FROM;
done;
