#!/bin/bash
set -xeuo pipefail
# code start here.
function __readINI() {
  local INIFILE=$1
  local SECTION=$2
  local ITEM=$3
  _readIni=$(awk -F '=' '/\['$SECTION'\]/{a=1}a==1&&$1~/'$ITEM'/{print $2;exit}' $INIFILE)
  echo ${_readIni}
}

function __init() {
  make clean
  make
}

function __run_test() {
  $1 $2 $3
}

function main() {
  if [ "$(__readINI ./test/config.ini func init)" = "1" ]; then
    __init
  fi
  local c_conf_url=$(__readINI ./test/config.ini conf c_url)
  local p_conf_url=$(__readINI ./test/config.ini conf p_url)
  if [ "$(__readINI ./test/config.ini func run_test)" = "1" ]; then
    __run_test ./kvstore2pcsystem --config_path ./test/${c_conf_url}
  fi
}

main

# code end here.
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM EXIT
