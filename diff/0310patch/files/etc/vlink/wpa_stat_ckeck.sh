#!/bin/sh

case "$2" in
	CONNECTED)
		#sendnetworkcmd networkok
		echo "wpa_connect-----CONNECTED-------success" > /dev/console
		#ps -w | grep udhcpc | awk '{print $1}' | xargs kill -9
		#udhcpc -i rai0 -p /tmp/dxs/udhcpc.pid -s /etc/dxs/udhcpc-default.script -h LenovoSmartAir
		#/etc/lenovo/checkinternet.sh
	;;
	DISCONNECTED)
		#sendnetworkcmd networkfail
		echo "wpa_connect-----DISCONNECTED-------fail" > /dev/console
	;;
esac
