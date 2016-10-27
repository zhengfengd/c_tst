#---------------------------------------------------------------------
# 09Aug01 pbr	Change so that real user is different from effective user,

#		the .cshrc of the real user is sourced and the junk in
#		this .cshrc is ignored.
#         cst   Temporarily change settings for AREA definitions
#               to keep compatibility with startup procedure
# 30Mar01 cst   Re-do the modification for PRS-1580 since it has been cancelled
#               The sourced file must be /products/csrhc in order to  
#               centralize all the definitions related to the TCS SW
# 06Jun01 pan   Set this (~tcsadmin) as reference for all developpers.
#               Included changes by ACH.
#---------------------------------------------------------------------
# Debug

## pbr fix

if ( $?tcsh ) then
    if ( $shlvl == 1 ) then
        if ( "`tty`" == "not a tty" ) then
            exit
        else
            setenv REALME `/usr/bin/whoami | /usr/bin/cut -f 1 -d ' '`
            echo "sourcing ~${REALME}/.cshrc"
        endif
    endif
endif

if ( ${?REALME} ) then
    if ( ${REALME} != $user ) then
        source ~${REALME}/.cshrc
        exit
    endif
endif

## pbr end

if (! $?tcsh && ( -f /bin/tcsh || -f /usr/local/GNU/bin/tcsh )) then
  if -f /bin/tcsh then
    setenv SHELL /bin/tcsh
  else
    setenv SHELL /usr/local/GNU/bin/tcsh
  endif
endif
#set path=( $path ~/tools)
#set path=( $path /products/ccase/bin)

#############################################################
# The next one has been changed by EDE and CST for PRS_1580 # #source ~/cshrc

# Status messages or not (when in common cshrc) ?
#------------------------------------------------
# 0 = Login is verbose
# 1 = Login is silent (needed for CU kernel loads) setenv HUSH_LOGIN 0


# Set the desired Tornado version:
#---------------------------------
# 0 = no tornado, trust Makefiles
# 1 = Tornado 1 (for TCS V1 - default)
# 2 = Tornado 2 (for TCS V1bis)
setenv TORNADO 0

# Set the desired Python version:
#---------------------------------
# 0 = no python
# 1 = Python 1.5.2 (for TCS V1 - default) # 2 = Python 2.2 (for TCS V1bis) setenv PYVER 2

# Set the desired RTworks version:
#---------------------------------
#  0 = no rtworks, trust Makefiles
#  3 = RTworks 3.5 (for TCS V1)
#  4 = RTworks 4.0 (for TCS V1bis - default) # 11 = RTworks 4.0 for 11.00 (for TCS V1bis) setenv RTVERSION 11

# Select the desired Oracle environment:
#---------------------------------------
#  0 = no Oracle, trust Makefiles
#  1 = Oracle runtime (RDBMS 8.0.5 - default) #  2 = Oracle development (Dev2K) #setenv ORAENVTYPE 2 setenv ORAENVTYPE 1

# Select the desired Java environment:
#-------------------------------------
#  0 = use java libs from /products (default) #  1 = use java libs from COTS VOB setenv INVOBJLIBS 1

if ( -f /products/ibanet/cshrc ) then
  source /products/ibanet/cshrc
endif

# CST temporary for backward compatibility with startup procedure setenv REF_AREA /CCASE/vobs/TCS_MGH setenv USER_AREA /CCASE/vobs/TCS_MGH

#To use operator on a qwerty keyboard
setenv OPERATOR_KEYBOARD QWERTY_US

#setenv EDITOR /usr/local/bin/nedit
setenv EDITOR /usr/bin/vi
#setenv WINEDITOR /usr/local/bin/nedit
setenv WINEDITOR /usr/bin/vi

#etenv WINEDITOR /usr/local/bin/neditww Set up C shell environment:
if ( $?prompt ) then		# shell is interactive.

    set history=50		# previous commands to remember.
    set savehist=50		# number to save across sessions.
    set system=`hostname`	# name of this system.
    set prompt = "$system \!: " # command prompt.
    set prompt = "`hostname`.`whoami` `dirs`>"
    
    # Sample alias:
    alias cd    'set olddir=$cwd ; chdir \!:* ; set prompt = "`hostname`.`whoami` `dirs`>"'
    alias h	history		
    alias lsa 	'ls -al'
    alias title 'echo -n " ]0;\!* "'
endif
setenv PATH /usr/local/bin:$PATH
setenv PATH /products/jakarta-ant/bin:$PATH # TCS aliases alias ct cleartool alias cl clearlicense

alias ccecu  'cd $REF_AREA/src/cu/ecu_btcu'
alias cctcu  'cd $REF_AREA/src/cu/tcu'
alias ccpcu  'cd $REF_AREA/src/cu/pcu'
alias ccsys  'cd $REF_AREA/src/app/systemMng'
alias cctrm  'cd $REF_AREA/src/app/treatmentmgr'
alias ccbeam 'cd $REF_AREA/src/app/beammgr'
alias ccdata 'cd $REF_AREA/src/datadaq'

# DISPLAY aliases

alias launch 'launch | & cat > /dev/null &'

#
# Settings for compiling V1
#
# 28/05/2001 by ACH
#
alias setview 'setenv REF_AREA /view/\!*/CCASE/vobs/TCS_MGH; setenv USER_AREA $REF_AREA; setenv MAKEDEF $REF_AREA; echo "Using $REF_AREA ..."'

#Set some Clearcase aliases
  alias sv "cd $TCS_ROOT && ct setview"
  alias cdv "cd ${TCS_ROOT}/\!*"
  alias cdmake "cdv src/make/\!*"
  alias cdapp "cdv src/app/\!*"
  alias cdhci "cdv src/hci/\!*"
  alias cdcu "cdv src/cu/\!*"
  alias cdpcu "cdcu pcu-schaer/\!*"
  alias cdppscu "cdcu ppscu/\!*"
  alias cdpps "cdppscu pps/\!*"
  alias cdecu "cdcu ecu_btcu/\!*"
  alias cdtcu "cdcu tcu/\!*"
  alias cdruntime "cdv runtime/\!*"
  alias cdvxworks "cdruntime vxWorks/\!*"
  alias cdint "cdv integration/\!*"
  alias cdproc "cdint procedures/\!*"
  
  # 21/12/2000 ach
   # Set up the terminal:
   setenv TERM xterm
   stty erase "^H" kill "^U" intr "^C" eof "^D"
   stty hupcl ixon ixoff echoe
   tabs
   
   #Set some aliases
  alias rm 'rm -i'
  alias cp 'cp -i'
  alias mv 'mv -i'
  alias ct cleartool
  # Search string
  alias ss  "find . -type f | xargs grep \!*"
  alias ssc "find . -name '*.c' | xargs grep \!*"
  alias ssh "find . -name '*.h' | xargs grep \!*"
  alias ssx "find . -name '*.[hc]' | xargs grep \!*"
  alias ssm "find . -name '[Mm]ake*' | xargs grep \!*"
  
  # Enhanced search string
  alias ess  "find . -type f | grep -vi test | grep -vi stub | xargs grep \!*"
  alias essc "find . -name '*.c' | grep -vi test | grep -vi stub | xargs grep \!*"
  alias essh "find . -name '*.h' | grep -vi test | grep -vi stub | xargs grep \!*"
  alias essx "find . -name '*.[hc]' | grep -vi test | grep -vi stub | xargs grep\!*"
  alias essm "find . -name '[Mm]ake*' | grep -vi test | grep -vi stub | xargs grep \!*"
  
  alias ctfind "cleartool find -all -branch 'brtype(\!:1)' -print"
  alias ctfindn "cleartool find -all -branch 'brtype(\!:1)' -nxn -print"
  
  alias ctfindlbl "cleartool find -all -type file -version 'lbtype(\!:1)' -print"
  
  alias ctdifflbl "cleartool find -all -type file -version '\!lbtype(!:1) && lbtype(!:1)' -print"
  alias ctfindbranch "cleartool lstype -kind brtype -short | grep \!*"
   
setenv DISPLAY BE1104222L.lln.iba:0.0
#setenv DISPLAY 10.1.39.149:0.0



