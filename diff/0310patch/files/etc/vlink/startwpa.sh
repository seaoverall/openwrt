#!/bin/sh

#mount -t nfs -o nolock 192.168.1.2:/home/hcw/data/work/hisisdk/nfc_root /mnt
#wpa_cli -i wlan0 status | grep wpa_state
# wpa_supplicant -D ipw -c /etc/vlink/wpa_supplicant.conf -i wlan0 &

insmod /mnt/rtl8188etv/rtl8188eu.ko
ifconfig wlan0 up

wpa_supplicant -iwlan0 -Dwext -c /etc/vlink/wpa_supplicant.conf &

cp /mnt/rtl8188etv/udhcpc-default.script /etc
chmod +x /etc/udhcpc-default.script
udhcpc -i wlan0 -s /etc/udhcpc-default.script &


wpa_supplicant ¨CDnl80211 -iwlan0 -c /etc/vlink/wpa_supplicant.conf ¨CB
or
wpa_supplicant -Dwext -iwlan0 -c /etc/vlink/wpa_supplicant.conf -B


wpa_cli -p/var/run/wpa_supplicant remove_network 0
wpa_cli -p/var/run/wpa_supplicant ap_scan 1
wpa_cli -p/var/run/wpa_supplicant add_network
wpa_cli -p/var/run/wpa_supplicant set_network 0 ssid '"dlink"'
wpa_cli -p/var/run/wpa_supplicant set_network 0 key_mgmt NONE
wpa_cli -p/var/run/wpa_supplicant select_network 0






