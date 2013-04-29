#!/bin/bash

INETD="/usr/bin/simple_inetd"
PIDFILE="/var/run/simple_inetd.pid"
CONF="/etc/simple_inetd/simple_inetd.conf"
PGREP="/usr/bin/pgrep"
KILL="/bin/kill"


function check_execute_bit() {
  test -x "$INETD"
  if [[ "$?" -ne 0 ]]; then
    echo "$INETD is not executable! Exit."
    exit 1
  fi
}


function start_daemon() {
  echo -n "Starting simple inetd ... "
  $INETD "$CONF"
  case "${?}" in
    0)
      sleep 2
      PID=$($PGREP -f "$INETD")
      if [[ ! -z "$PID" ]]; then
        echo "Done!"
      else
        echo "Failed!"
      fi
      ;;
    *)
      echo "Failed!"
      ;;
  esac
}


function stop_daemon() {
  PID=$($PGREP -f "$INETD")
  if [[ ! -z "$PID" ]]; then
    echo -n "Stopping simple inetd ... "
    $KILL -TERM "$PID"
    sleep 2
    PID=$($PGREP -f "$INETD")
    if [[ ! -z "$PID" ]]; then
      echo "Failed!"
    else
      echo "Done!"
    fi
  else
    echo "Simple inetd is not running ... Done!"
  fi
}


function reload_daemon() {
  PID=$($PGREP -f "$INETD")
  if [[ ! -z "$PID" ]]; then
    echo -n "Sending singnal HUP to simple inetd ... "
    $KILL -HUP "$PID"
    ret=$?
    sleep 2
    if [[ "$ret" -eq 0 ]]; then
      echo "Done!"
    else
      echo "Failed!"
    fi
  fi

}


function restart_daemon() {
  stop_daemon
  start_daemon
}


# Main logic starts here.
check_execute_bit

case "$1" in
  start)
    start_daemon
    ;;
  stop)
    stop_daemon
    ;;
  reload)
    reload_daemon
    ;;
  restart)
    restart_daemon
    ;;
  *)
    echo "Usage: $0 [start|stop|restart|reload]"
    ;;
esac
