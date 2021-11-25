#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mt7601

PKG_RELEASE:=v1.9

include $(INCLUDE_DIR)/package.mk

WMENU:=Wireless Drivers

define KernelPackage/mt7601
	SUBMENU:=Wireless Drivers
	TITLE:=Driver for MT7601U wireless adapters
	FILES:=$(PKG_BUILD_DIR)/os/linux/mt7601Usta.$(LINUX_KMOD_SUFFIX)
	DEPENDS:=+wireless-tools @USB_SUPPORT +kmod-usb-core
	AUTOLOAD:=$(call AutoLoad,90,mt7601Usta)
	MENU:=1
endef

define KernelPackage/mt7601/config
  if PACKAGE_kmod-mt7601
  
	config PACKAGE_MT7601_DEBUG
		bool "Enable debug messages"
		default n
		help
		  With this option the driver will emit A LOT
		  of debugging information. 
		  The driver is quite noisy, so enable it only if
		  absolutely necessary to debug problems with this
		  driver.
		  
  endif
endef

define KernelPackage/mt7601/description
  This package contains a driver for usb wireless adapters based on the mediatek MT7601U
endef

ifneq ($(CONFIG_BIG_ENDIAN),)
  ENDIANOPTS:=-DRT_BIG_ENDIAN
else
  ENDIANOPTS:=
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) LINUX_DIR=$(LINUX_DIR) KERNEL_CROSS=$(KERNEL_CROSS) ARCH=$(LINUX_KARCH) ENDIANOPTS=$(ENDIANOPTS) RTDEBUG=$(CONFIG_PACKAGE_MT7601_DEBUG)
endef

define KernelPackage/mt7601/install
	$(INSTALL_DIR) $(1)/etc/Wireless/RT2870STA/
	$(INSTALL_DIR) $(1)/lib/modules/$(LINUX_VERSION)
	$(INSTALL_DIR) $(1)/lib/wifi
	$(CP) $(PKG_BUILD_DIR)/MT7601USTA.dat $(1)/etc/Wireless/RT2870STA/
endef

$(eval $(call KernelPackage,mt7601))
