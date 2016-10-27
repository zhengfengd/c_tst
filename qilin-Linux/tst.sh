#!/bin/sh
i=0;
step=30;
if [ $# == 1 ]; then
  step=`expr $1`;
fi

echo input is $step, i is $i;

while [ $i -lt 360  ]; do
        echo $i;
        #convert -rotate $i 4Circles.png 4Circles.$i.png
        echo 4Circles.$i.png
        i=`expr $i + $step`;        
done;

#display -delay 5 4Circles.*.png
echo "the following result:"
for item in `ls 4Circles.*.png`; do
	echo $item;
done;
