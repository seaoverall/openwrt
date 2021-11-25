#!/bin/sh

echo "-----connectwlan.sh--------start" > /dev/console
mkdir -p /tmp/vlink/

USBINFO=`lsusb | grep Ralink`
USBINFOLEN=`echo -n $USBINFO | wc -c`

echo "-----connectwlan.sh:len:$USBINFOLEN--------1" > /dev/console

if [ $USBINFOLEN -gt 0 ]; then 
	echo $USBINFO > /dev/console
	wpa_supplicant -iwlan0 -Dnl80211 -c /etc/vlink/wpa_supplicant_psk.conf &
	iwpriv wlan0 set Debug=0

	wpa_cli -i wlan0 -P /tmp/vlink/wpacli.pid -a /etc/vlink/wpa_stat_ckeck.sh -B
	udhcpc -i wlan0 -p /tmp/vlink/udhcpc.pid -s /etc/vlink/udhcpc-default.script -h vlink

else
	wpa_supplicant -iwlan0 -Dwext -c /etc/vlink/wpa_supplicant_psk.conf &	
fi


#wpa_supplicant -iwlan0 -Dwext -c /etc/vlink/wpa_supplicant_open.conf &

#wpa_supplicant -iwlan0 -Dnl80211 -c /etc/vlink/wpa_supplicant_psk.conf -B



