#!/bin/bash 
echo "Wrapper to send actions to arduino car"
echo "Here we go: $@"


PTOOL="/opt/carem/piInoAgent/piInoagentWrapper.sh"

################################################
function usage
{ 
  echo  "$0 
           --dance
           --hello
           --help"
  echo "Notes:
           --color will change main color theme on led strip
           <COLOR> should be a color known by python colour module or #RRGGBB html notation  
"
}


################################################
function sendLSRaw 
{
  curl -i -H "Content-Type: application/json" -X POST -d '{"raw":"'$1'"}' http://localhost:5001/api/v1.0/ls/raw
} 

################################################
function dance
{
  $PTOOL --mode fullColor

  for i in `seq 1 2`;
  do
    $PTOOL --color red
    sendLSRaw ":EMB"
    sleep 0.2
    sendLSRaw ":EMF"
    sleep 0.2
    sendLSRaw ":EMR"
    sleep 0.2
    sendLSRaw ":EML"
    sleep 0.2
    $PTOOL --color blue
    sendLSRaw ":EU"
    sleep 0.5
  done 

  $PTOOL --color green
  sleep 1
  $PTOOL --off
  sendLSRaw ":EMS"
} 
################################################
function hello
{
  $PTOOL --mode knightRider
  $PTOOL --pause 50
  sleep 0.8
  $PTOOL --off
} 
################################################
################################################
################################################
if [ "$1" == "-h" -o "$1" == "--help" ]; then
  usage
  exit 0
elif [ "$1" == "" ]; then
  echo "ERROR, args needed"
  usage
  exit -1
elif [ "$1" == "--dance" ]; then
  dance 
  exit 0
elif [ "$1" == "--hello" ]; then
  hello 
  exit 0
else
  echo "ERROR, unknown option $1"
  usage
  exit -1
fi






