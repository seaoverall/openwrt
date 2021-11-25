# All rights reserved.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=rtl8188FU
PKG_RELEASE=5.3.0.1

include $(INCLUDE_DIR)/package.mk

TAR_CMD=$(HOST_TAR) -C $(1)/ $(TAR_OPTIONS)

WMENU:=Wireless Drivers

define KernelPackage/rtl8188FU
	SUBMENU:=Wireless Drivers
	TITLE:=Driver for rtl8188FTV wireless adapters
	FILES:=$(PKG_BUILD_DIR)/8188fu.$(LINUX_KMOD_SUFFIX)
	DEPENDS:=+wireless-tools @USB_SUPPORT +kmod-usb-core
	AUTOLOAD:=$(call AutoLoad,90,8188fu)
	MENU:=1
endef

define KernelPackage/rtl8188FU/description
  This package contains a driver for usb wireless adapters based on the rtl8188FTV
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) LINUX_DIR=$(LINUX_DIR) KERNEL_CROSS=$(KERNEL_CROSS) ARCH=$(LINUX_KARCH)
endef

define KernelPackage/rtl8188FU/install
	$(INSTALL_DIR) $(1)/lib/modules/$(LINUX_VERSION)
endef

$(eval $(call KernelPackage,rtl8188FU))
