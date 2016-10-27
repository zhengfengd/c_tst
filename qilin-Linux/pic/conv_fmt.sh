#!/bin/sh

echo "the following result:"

for FROM in `ls IMG_*.JPG`;
do
	echo $FROM;
	convert -sample 400 $FROM new.$FROM
	convert -font fonts/1900805.ttf -fill red -pointsize 20 \
		-draw 'text 225,265 "IBA KNCC TEAM"' \
		new.$FROM logo.new.$FROM 
	#echo "$FROM -> new.$FROM";
done;
