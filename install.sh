#!/bin/bash

# $Id: install.sh,v 1.56 2004/11/20 13:47:07 christhecat Exp $

#===================================================
# If you want to install the server in a directory other than
# $HOME/Games, change the following line, and add the line
#   export GALAXYNGHOME=<the dir you install it in> 
# to your .bash_profile file.
#===================================================

GALAXY_HOME=$HOME/Games

echo "*** GalaxyNG Install Script ***"
echo "This script creates and installs all files necessary to"
echo "run Galaxy Games.  Files will be installed in"
echo "$GALAXY_HOME"
echo

#===================================================
#  Create directories, if necessary.
#===================================================

if [ ! -d $GALAXY_HOME ]; then
  echo "Creating $GALAXY_HOME"
  mkdir -p $GALAXY_HOME
fi
for NAME in log orders notices data reports statistics forecasts ; do
  if [ ! -d $GALAXY_HOME/$NAME ]; then
    mkdir $GALAXY_HOME/$NAME
  fi
done

#===================================================
#  Determine GM and server email addresses and password.
#===================================================

echo "o What is the GM email address?"
echo "  (Email to the GM is sent to this address)"
read GMEMAIL
echo "o What is the GM name?"
echo "  (This is used on outgoing mail)"
read GMNAME
echo "o What is the server email address?"
echo "  (galaxyng commands are sent to this address)"
read SERVEREMAIL
echo "o What is the server name?"
echo "  (This is used on outgoing mail)"
read SERVERNAME
echo "o What is the GM password?"
echo "  (used for relay commands)"
echo "  DO NOT USE THE GM ACCOUNT PASSWORD.  THIS PASSWORD"
echo "  IS NOT SECURE!"
read PASSWORD

#====================================================
#  Try to find the sendmail command.
#====================================================

echo "o Trying to locate the sendmail command..." 
# Check the usual locations,  "which" does not work usually.
for NAME in /bin /sbin /usr/sbin /usr/bin ; do
  echo "  Checking: $NAME"
  if [ -x $NAME/sendmail ]; then 
    SENDMAIL=$NAME/sendmail
    break
  fi 
done
# If it was not found, ask the user where it is
if [ -z "$SENDMAIL" ]; then
  echo "  I can't seem to find the sendmail command."
  echo "  Please enter the full path for sendmail:"
  read SENDMAIL
fi
echo "  The full path for sendmail is:"
echo "  $SENDMAIL"

# =========================================================
#  Try to find the formail command
# =========================================================

echo "o Trying to locate the formail command." 
# Check the usual locations
for NAME in /bin /sbin /usr/sbin /usr/bin ; do
  echo "  Checking: $NAME"
  if [ -x $NAME/formail ]; then  
    FORMAIL=$NAME/formail
    break
  fi 
done
# If it was not found, try which
if [ -z "$FORMAIL" ]; then
	FORMAIL=`which formail`
fi
# If it was not found, ask the user
if [ -z "$FORMAIL" ]; then
  echo "  I can't seem to find the formail command."
  echo "  This may mean you don't have procmail installed."
  echo "  Please enter the full path to formail:"
  read FORMAIL
fi
echo "  The full path for formail is:"
echo "  $FORMAIL"

# =========================================================
#  Find the compression utility
# =========================================================

echo "o Trying to find the compression utility."
COMPRESS=`which zip`
if [ -z "$COMPRESS" ]; then
  echo "  I can't seem to find the compression utility."
  echo "  Please enter the full path for the compression utility:"
  read COMPRESS
fi
echo "  The full path for the compression utility is:"
echo "  $COMPRESS"

# =========================================================
#  Find the mime encoder
# =========================================================

echo "o Trying to find the mime encoder."
for NAME in mmencode mimencode; do
  echo "  Checking: $NAME"
  ENCODE=`which $NAME`
  if [ -n "$ENCODE" ]; then
    break;
  fi
done
if [ -z "$ENCODE" ]; then
  echo "  I can't seem to find the mime encoder."
  echo "  Please enter the full path for the mime encoder:"
  read ENCODE
fi
echo "  The full path for the mime encoder is:"
echo "  $ENCODE"

# ===========================================================
#  Try to find the location of the public web directory.
# ===========================================================

echo "o Trying to locate the web directory.  GalaxyNG uses this"
echo "  directory to store the high score lists."
for NAME in $HOME/public_html $HOME/web $HOME/www $HOME/WWW; do
  echo "  Checking: $NAME"
  if [ -d $NAME ]; then 
    WWW=$NAME
    break
  fi
done
if [ -z "$WWW" ]; then
  echo "  I can't seem to find the your web directory,"
  echo "  Please enter the full path to your web directory:"
  read WWW
  if [ ! -d $WWW ]; then
    mkdir $WWW
  fi
fi
echo "  The web directory is:"
echo "  $WWW"

# =========================================================
#  Check for a /tmp directory
# =========================================================

echo "o Trying to locate a tmp directory."
if [ -d /tmp ]; then 
  TEMP=/tmp
else
  TEMP=$GALAXY_HOME/tmp
  mkdir $GALAXY_HOME/tmp
fi
echo "  The tmp directory is: $TEMP"

# =========================================================
#  Fonts for influence mapping
# =========================================================

if [ -f Source/angostur.ttf ]; then
	cp Source/angostur.ttf $GALAXY_HOME/influence.ttf
else
	cp Source/cranberr.ttf $GALAXY_HOME/influence.ttf
fi

# ========================================================
#  Create run_game
# ========================================================

RUN_GAME=$GALAXY_HOME/run_game
echo "o Creating $RUN_GAME"
#echo "  The run_game script is used to run a turn."
if [ -e $RUN_GAME ]; then
  echo "  Found an existing version of $RUN_GAME"
  RUN_GAME=$GALAXY_HOME/run_game.new
  echo "  Writing the new version to $RUN_GAME"
fi

echo "#!/bin/bash" > $RUN_GAME
cat Util/run_game.header >> $RUN_GAME
echo "BASE=$GALAXY_HOME" >> $RUN_GAME
echo "WWWHOME=$WWW" >> $RUN_GAME
echo "GALAXYNGHOME=$GALAXY_HOME" >> $RUN_GAME
echo "declare -x GDFONTPATH=$GALAXY_HOME" >> $RUN_GAME
cat Util/run_game.tail >> $RUN_GAME
chmod +x $RUN_GAME

# =========================================================
#  Create example .procmailrc file
# =========================================================

PROCRC=$GALAXY_HOME/procmailrc

echo "o Creating $PROCRC"
echo "  You can use this file in combination with procmail"
echo "  to automatically check incoming orders."
echo "  To use it copy it to $HOME as $HOME/.procmailrc"
echo "  Note that is $HOME/[dot]procmailrc"
if [ -e $PROCRC ]; then
  echo "  Found an existing version of $PROCRC"
  PROCRC=$GALAXY_HOME/procmailrc.new
  echo "  Writing the new version to $PROCRC"
fi

echo "PATH=\$HOME/bin:$GALAXY_HOME:/usr/bin:/bin:/usr/local/bin:." > $PROCRC
echo "#" >> $PROCRC
echo "# Make sure that your mail directory exists!" >> $PROCRC
echo "MAILDIR=\$HOME/Mail" >> $PROCRC
echo >> $PROCRC
echo "DEFAULT=\$MAILDIR/mbox" >> $PROCRC
echo "# For maildir delivery, mailboxes end in a /, so comment" >> $PROCRC
echo "# out the above line and use this instead:" >> $PROCRC
echo "# DEFAULT=\$MAILDIR/" >> $PROCRC
echo >> $PROCRC
echo "LOGFILE=\$HOME/procmail.log" >> $PROCRC
echo "LOCKFILE=\$HOME/.lockmail" >> $PROCRC
echo "LOGABSTRACT=all" >> $PROCRC
echo "GALAXYNGHOME="$GALAXY_HOME >> $PROCRC
echo >> $PROCRC
echo "# Store GM reports in a folder called gmreport." >> $PROCRC
echo ":0:" >> $PROCRC
echo "* ^Subject:.*GM Report" >> $PROCRC
echo "gmreport" >> $PROCRC
echo "# Some IMAP servers prefix mailbox names with a dot." >> $PROCRC
echo "#  For example:" >> $PROCRC
echo "# .gmreport" >> $PROCRC
echo "# Combining this with maildir delivery would yield:" >> $PROCRC
echo "# .gmreport/" >> $PROCRC
echo "# Make sure to change the folders listed below to match" >> $PROCRC
echo "# your configuration." >> $PROCRC
echo >> $PROCRC
echo "# Store orders due messages in a folder called due." >> $PROCRC
echo ":0:" >> $PROCRC
echo "* ^Subject:.*is about to run." >> $PROCRC
echo "due" >> $PROCRC
echo >> $PROCRC
echo "# Don't reply to anything from a mail daemon, but store it" >> $PROCRC
echo "# in a folder called postmaster." >> $PROCRC
echo ":0:" >> $PROCRC
echo "* ^FROM_MAILER" >> $PROCRC
echo "postmaster" >> $PROCRC
echo >> $PROCRC
echo "# The following prevents mail loops. These happen when the" >> $PROCRC
echo "# server starts replying to its own messages or messages" >> $PROCRC
echo "# from another server. Mail loops usually annoy the heck" >> $PROCRC
echo "# out of sysadmins." >> $PROCRC
echo ":0:" >> $PROCRC
echo "* ^Subject:.*(major trouble|message sent|message not sent|received for|report for|forecast for|report copy)" >> $PROCRC
echo "loops" >> $PROCRC
echo >> $PROCRC
echo "# Received a message with the word order in the subject:" >> $PROCRC
echo ":0" >> $PROCRC
echo "* ^Subject:.*order" >> $PROCRC
echo "{" >> $PROCRC
echo "  :0crw:order" >> $PROCRC
echo "  # Check the orders and send a forecast or an error message:" >> $PROCRC
echo "  |$FORMAIL -rkbt -s $GALAXY_HOME/galaxyng -check" >> $PROCRC
echo "  :0:orders" >> $PROCRC
echo "  # Save a copy of the orders message" >> $PROCRC
echo "  orders" >> $PROCRC
echo "}" >> $PROCRC
echo >> $PROCRC
echo "# Received a message with the word report in the subject:" >> $PROCRC
echo ":0" >> $PROCRC
echo "* ^Subject:.*report" >> $PROCRC
echo "{" >> $PROCRC
echo "  # Send a turn report or an error message: " >> $PROCRC
echo "  :0crw:report" >> $PROCRC
echo "  |$FORMAIL -rkbt -s $GALAXY_HOME/galaxyng -report" >> $PROCRC
echo "  :0:reports" >> $PROCRC
echo "  # Save a copy of the report request" >> $PROCRC
echo "  reports" >> $PROCRC
echo "}" >> $PROCRC
echo >> $PROCRC
echo "# Someone wants to relay a message to another player." >> $PROCRC
echo ":0" >> $PROCRC
echo "* ^Subject:.*relay" >> $PROCRC
echo "{" >> $PROCRC
echo "  :0crw:relay" >> $PROCRC
echo "  # Relay the message and send a confirmation or error report to the player" >> $PROCRC
echo "  |$FORMAIL -rkbt -s $GALAXY_HOME/galaxyng -relay" >> $PROCRC
echo "  :0:relays" >> $PROCRC
echo "  # Save a copy of the relay request" >> $PROCRC
echo "  relays" >> $PROCRC
echo "}" >> $PROCRC
echo >> $PROCRC
echo "# Someone wants to sign up for a game.  Don't forget to" >> $PROCRC
echo "# change the gamename and number of players." >> $PROCRC
echo ":0" >> $PROCRC
echo "* ^Subject.*Join Jangi" >> $PROCRC
echo "{" >> $PROCRC
echo "  :0c:Jangi" >> $PROCRC
echo "  |$FORMAIL -rbt -s $GALAXY_HOME/are Jangi 25 0 0 0 | $SENDMAIL -t" >> $PROCRC
echo "  :0:Jangi" >> $PROCRC
echo "  # Save a copy of the registration request" >> $PROCRC
echo "  Jangi" >> $PROCRC
echo "}" >> $PROCRC
echo >> $PROCRC
echo "# Someone wants to sign up for a game with custom planet" >> $PROCRC
echo "# sizes.Up to ten players, max 2500 production, max planet" >> $PROCRC
echo "# size 1000, max planets 5.  Don't forget to change the" >> $PROCRC
echo "# gamename, planet sizes, and number of players." >> $PROCRC
echo ":0" >> $PROCRC
echo "* ^Subject.*Join Welland" >> $PROCRC
echo "{" >> $PROCRC
echo "  :0c:Welland" >> $PROCRC
echo "  |$FORMAIL -rbt -s $GALAXY_HOME/are Welland 10 2500 1000 5 | $SENDMAIL -t" >> $PROCRC
echo "  :0:Welland" >> $PROCRC
echo "  # Save a copy of the registration request" >> $PROCRC
echo "  Welland" >> $PROCRC
echo "}" >> $PROCRC
echo >> $PROCRC
echo "# Anything else, or messages that cause the engine to fail" >> $PROCRC
echo "# are stored in the default mailbox which was set at the" >> $PROCRC
echo "# top of this file." >> $PROCRC

# =========================================================
#  Create example crontab file
# =========================================================

CRONT=$GALAXY_HOME/games.crontab
echo "o Creating $CRONT"
echo "  Edit it and run: crontab games.crontab"
if [ -e $CRONT ]; then
  echo "  Found an existing version of $CRONT"
  CRONT=$GALAXY_HOME/games.crontab.new
  echo "  Writing the new version to $CRONT"
fi
echo "# This is an example crontab file. It would run the game " > $CRONT
echo "# Jangi on Monday, Wednesday, and Friday at 21:15 (9:15pm)" >> $CRONT
echo "# and the game Welland on Tuesday and Friday at 08:00 (8:00am)" >> $CRONT
echo "# It will also send 'orders due' messages four hours before" >> $CRONT
echo "# each turn.  To use it, type" >> $CRONT
echo "#   crontab games.crontab" >> $CRONT 
echo >> $CRONT
echo "15 21 * * 1,3,5 $RUN_GAME Jangi" >> $CRONT
echo "15 17 * * 1,3,5 $GALAXY_HOME/galaxyng -due Jangi" >> $CRONT 
echo "0 8 * * 2,5 $RUN_GAME Welland" >> $CRONT
echo "0 4 * * 2,5 $GALAXY_HOME/galaxyng -due Welland" >> $CRONT 

# =========================================================
#  Create a .galaxyngrc file
# =========================================================

RCFILE=$GALAXY_HOME/.galaxyngrc
echo "o Creating $RCFILE"
echo "  This file contains basic configuration information for the server."
echo "  You can edit it by hand. You can put game specific .galaxyngrc"
echo "  files in the game data directories.  See the server manual for"
echo "  more information. Note that is $GALAXY_HOME/[dot]galaxyngrc"
if [ -e $RCFILE ]; then
  echo "  Found an existing version of $RCFILE"
  RCFILE=$GALAXY_HOME/.galaxyngrc.new
  echo "  Writing the new version to $RCFILE"
fi
echo "; This is the global galaxyng configuration file." > $RCFILE
echo "; You can override this file for a game by putting" >> $RCFILE
echo "; a .galaxyngrc file in the game data directory:" >> $RCFILE
echo "; $GALAXY_HOME/data/<gamename>" >> $RCFILE
echo "sendmail { $SENDMAIL -t }" >> $RCFILE
echo "GMemail $GMEMAIL" >> $RCFILE
echo "GMname $GMNAME" >> $RCFILE
echo "GMpassword $PASSWORD" >> $RCFILE
echo "SERVERemail $SERVEREMAIL" >> $RCFILE
echo "SERVERname $SERVERNAME" >> $RCFILE
echo "encode $ENCODE" >> $RCFILE
echo "compress $COMPRESS" >> $RCFILE
echo "fontpath $GALAXY_HOME" >> $RCFILE
echo "; The due parameter (hours) is used by the -immediate and -due flag." >> $RCFILE
echo "due 4" >> $RCFILE
echo "; The tick parameter (hours) is used by the -immediate flag." >> $RCFILE
echo "tick 48" >> $RCFILE

# =========================================================
#  Copy GalaxyNG code
# =========================================================

echo "o Copying the GalaxyNG server"
NG=$GALAXY_HOME/galaxyng
if [ -e $NG ]; then
  echo "  Found an existing version of " $NG
  NG=$GALAXY_HOME/galaxyng.new
  echo "  Writing the new version to "  $NG
  echo "  _Test_ this new version and if it works according to your needs"
  echo "  copy it to " $GALAXY_HOME "/galaxyng"
fi
cp Source/galaxyng $NG
strip $NG

# =========================================================
#  Copy Automatic Registration Engine (ARE) code
# =========================================================

echo "o Copying the Automatic Registration Engine (ARE)"
ARE=$GALAXY_HOME/are
if [ -e $ARE ]; then
  echo "  Found an existing version of " $ARE
  ARE=$GALAXY_HOME/are.new
  echo "  Writing the new version to "  $ARE
  echo "  _Test_ this new version and if it works according to your needs"
  echo "  copy it to " $GALAXY_HOME "/are"
fi
cp ARE/are $ARE
strip $ARE

# =========================================================
#  Finished
# =========================================================

echo "Installation completed."

