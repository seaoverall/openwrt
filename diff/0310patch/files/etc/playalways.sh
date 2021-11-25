#!/bin/sh

echo "-----playalways--------start" > /dev/console
 
tries=0
FILE="/etc/playalwaysrecord"
cp /etc/aaaa.mp3 /tmp/
cp /etc/bbbb.mp3 /tmp/
cp /etc/sound_test.wav /tmp/
sleep 60
while true
do

	sleep 1
	mp3playertest /tmp/aaaa.mp3
	tries=$((tries+1))
	sleep 1
	mp3playertest /tmp/bbbb.mp3
	sleep 1
	wavplayertest /tmp/sound_test.wav

	DATE=`cat /proc/uptime| awk -F. '{print $1}'`

	echo "$DATE--------------playalways:$tries-------------" >> $FILE
done
