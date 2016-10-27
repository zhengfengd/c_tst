xdialog --stdout  --password --title "Please enter user/password to enable the function" --inputbox2  "Set Local Workspace" 0 0
DEFPWD="testing"
input=`xdialog --stdout  --password --inputbox  "Set Local Workspace" 0 0 ""`
echo $input
if [ $input != $DEFPWD ];then
	echo "Invalid password, please try again"
	Xdialog --msgbox "Invalid password, please try again" 0 0
	exit 1
else
	echo "OK, you can go on"
fi

exit 0

    #FILE=`Xdialog --stdout --title "Please choose local setup file" --no-buttons  --fselect $PWD 0 0`
    FILE=`Xdialog --stdout -m "Must inside the directory!!" --title "Please enter inside local workspace" --no-buttons  --dselect "/home/zztudou/ctest" 0 0`
    retval=$?
	echo "return value is $FILE" 
	echo "second check"
	echo $FILE
	echo "retval is :"
	echo $retval
    if [ $? != 0 ]; then
	echo "no file is specified"
#	exit 1
    else
	echo "file is $FILE"
#        exit 0
    fi


LOCAL_CLASSPATH_USED=

Xdialog --title "Please enter into the directory to select the local workspace" --yesno "OK?" 0 0
    if [ $? == 0 ]; then
        echo $LOCAL_CLASSPATH_USED > localworkspace.ck
        Xdialog -m "The local workspace is un-selected, and now default site configuration becomes effective"
    else
        Xdialog -m "You cancel your operation, the local workspace is still there"
    fi
    
