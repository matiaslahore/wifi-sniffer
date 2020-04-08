#!/bin/bash
sleep 25

for a in $(ps -ef | egrep "cts-reciver.py|rts-sender.py" | grep -v "grep" | awk '{print $2}')
do
sudo kill -15 $a
done

sleep 2
