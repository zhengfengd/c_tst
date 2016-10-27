#SHELL = /bin/sh
gname = `hostname`

all:
	echo "all"

SET_LOCAL_PATH = 
start2:
	@echo $(gname)
	@-(Xdialog -m "TEST" -t "test" --yesno "Are you confirmed?" 0 0; \
	if [ $$? = 0 ]; then echo "just for test" ; \
	else echo "cancel it"; \
	fi)

start3:
ifneq ($(LOCAL_CLASSPATH_USED), )
	@-(Xdialog -m "!! WARNING: using local site config workspace $(LOCAL_CLASSPATH_USED) Do you confirm it?" \
	 -t "Continue patient treatment?" --yesno "continue" 0 0;  \t
	if [ $$? = 0];  \t
	then  \t
	@echo "SET LOCAL PATH:" \
	@echo $(SET_LOCAL_PATH) \
	else  \
	echo "you do not select local space, it will start with default site config" ; \
	fi;)
	@echo " !!! WARNING: using local site config workspace $(LOCAL_CLASSPATH_USED) - You should know what you are doing !!!"
else
	@echo "Normal steps"
endif

start:
	#@make -s stop_mcr_proc STOP_TYPE=gentle
ifneq ($(LOCAL_CLASSPATH_USED), )
	@echo "LOCAL PATH is used"
else
	@echo "LOCAL PATH is NOT there"
endif
	@echo "SET LOCAL PATH:"
	@echo $(SET_LOCAL_PATH)
	@echo "Please wait while system verifies start conditions..."	
