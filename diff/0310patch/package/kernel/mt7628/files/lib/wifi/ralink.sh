#!/bin/sh
#
# Copyright (c) 2014 OpenWrt
# Copyright (C) 2013-2015 D-Team Technology Co.,Ltd. ShenZhen
# Copyright (c) 2005-2015, lintel <lintel.huang@gmail.com>
# Copyright (c) 2013, Hoowa <hoowa.sun@gmail.com>
# Copyright (c) 2015, GuoGuo <gch981213@gmail.com>
#
# 	描述:Ralink/MTK ralink 2.4G无线驱动detect脚本
#
# 	嘿，对着屏幕的哥们,为了表示对原作者辛苦工作的尊重，任何引用跟借用都不允许你抹去所有作者的信息,请保留这段话。
#

append DRIVERS "mt7628"

. /lib/wifi/libmt7628.sh

detect_mt7628() {
	#判断系统是否存在ralink_ap相关模块，不存在则退出
	cd /sys/module/
		[ -d mt7628 ] || [ -d mt76x2e ] || [ -d mt7603e ]  || [ -d mt7628 ]  || return
		
	[ -d $CFG_FILES_DIR ] || mkdir -p $CFG_FILES_DIR
		
	#检查并创建WiFi驱动配置链接
	if [ $(cpu_is_mt7621) == "1" ]; then
	{
			[ -f /etc/Wireless/MT7603/MT7603.dat ] || {
				mkdir -p /etc/Wireless/MT7603/ 2>/dev/null
				touch $CFG_FILES_1ST
				ln -s $CFG_FILES_1ST /etc/Wireless/MT7603/MT7603.dat 2>/dev/null
			}
			[ -f /etc/Wireless/MT76X2/MT7602.dat ] || {
				mkdir -p /etc/Wireless/MT76X2/ 2>/dev/null
				touch $CFG_FILES_1ST
				ln -s $CFG_FILES_1ST /etc/Wireless/MT76X2/MT7602.dat 2>/dev/null
			}
	}
	else
	{
		[ -f /etc/Wireless/RT2860/RT2860.dat ] || {
			mkdir -p /etc/Wireless/RT2860/ 2>/dev/null
			touch $CFG_FILES_1ST
			ln -s $CFG_FILES_1ST /etc/Wireless/RT2860/RT2860.dat 2>/dev/null
		}
		[ -f /etc/wireless/mt7628/mt7628.dat ] || {
			mkdir -p /etc/wireless/mt7628/ 2>/dev/null
			touch $CFG_FILES_1ST
			ln -s $CFG_FILES_1ST /etc/wireless/mt7628/mt7628.dat 2>/dev/null
		}
	}
	fi;
	#检测系统是否存在ra0接口
	[ $( grep -c "ra0" /proc/net/dev) -eq 1 ] && {
		config_get type ra0 type
		[ "$type" = ralink ] && return

cat <<EOF
config wifi-device  ra0
	option type     	ralink
	option variant		mt7628
	option country		CN
	option hwmode		11g
	option htmode		HT40
	option channel  	auto

config wifi-iface ap
	option device		ra0
	option network		lan
	option ifname   	ra0
	option mode		ap
	option ssid		MT7628${i#0}_$( echo $(ralink_get_first_if_mac Factory) | awk -F ":" '{print $4""$5""$6 }'| tr a-z A-Z)
	option encryption	none
	
#	option encryption	psk2

config wifi-iface sta
	option device   	ra0
	option mode		sta
	option network  	wwan
	option ifname   	apcli0
	option ssid		""
	option key		""
	option encryption 	psk2
EOF
	}
}


