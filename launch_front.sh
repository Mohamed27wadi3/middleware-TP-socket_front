#!/bin/bash
set -e
ROOT="/home/kali/Desktop/TP Midleware"
SERVER="$ROOT/front_tp_web/server.py"
PORT=3000

if ! lsof -i :$PORT >/dev/null 2>&1; then
  nohup python3 "$SERVER" >/tmp/front_tp_web.log 2>&1 &
  sleep 0.8
fi

xdg-open "http://localhost:$PORT" >/dev/null 2>&1 &
