#
# Copyright (C) 2017 Chris Blake <chrisrblake93@gmail.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=led_gpio0
PKG_RELEASE:=1

PKG_MAINTAINER:=Chris Blake <chrisrblake93@gmail.com>
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/led_gpio0
  SUBMENU:=LED modules
  TITLE:= LED ON GPIO0
  DEPENDS:= @GPIO_SUPPORT @TARGET_x86
  FILES:=$(PKG_BUILD_DIR)/led_gpio0.ko
  AUTOLOAD:=$(call AutoLoad,41,led_gpio0,1)
  KCONFIG:=
endef

define KernelPackage/led/description
  test GPIO0 led on or off
endef

EXTRA_KCONFIG:= \
	CONFIG_LED_GPIO0=m

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Compile
	 $(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,led_gpio0))
