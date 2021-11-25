
#export PROD_DIR = $(TSPUB_DIR)/product/$(PROD_NAME)
#include $(TSPUB_DIR)/product/$(PROD_NAME)/os.mk
#include $(TOPDIR)/rules.mk
#ifeq ($(LITEOSTOPDIR), )
#	LITEOSTOPDIR := $(SDK_PATH)/osdrv/opensource/liteos/liteos
#endif

#include $(LITEOSTOPDIR)/config.mk
include $(TOPDIR)/rules.mk

BUILD := fdk_out

ARFLAGS = cr
LIBOUT = $(BUILD)/fdk-aac-0.1.5
LIBCRYPTO = ./libfdk-aac.a
RM = -rm -rf

LOCAL_INCLUDE := \
	-I ./libSBRdec/include \
	-I ./libMpegTPEnc/include \
	-I ./libAACenc/include \
	-I ./libPCMutils/include \
	-I ./libSYS/include \
	-I ./libSBRenc/include \
	-I ./libMpegTPDec/include \
	-I ./libAACdec/include \
	-I ./libFDK/include

SRCSCPP = $(wildcard libSBRdec/src/*.cpp)\
          $(wildcard libMpegTPEnc/src/*.cpp)\
		  $(wildcard libAACenc/src/*.cpp)\
		  $(wildcard libPCMutils/src/*.cpp)\
		  $(wildcard libSBRenc/src/*.cpp)\
		  $(wildcard libMpegTPDec/src/*.cpp)\
		  $(wildcard libAACdec/src/*.cpp)\
		  $(wildcard libFDK/src/*.cpp) \
		  $(wildcard libSYS/src/*.cpp)



#SRCSCPP += libFDK/src/arm/autocorr2nd.cpp  libFDK/src/arm/dct_arm.cpp  libFDK/src/arm/fft_rad2_arm.cpp\
           libFDK/src/arm/qmf_arm.cpp  libFDK/src/arm/scale_arm.cpp

OBJSC = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCSC))
OBJSCPP =  $(patsubst %.cpp,$(LIBOUT)/%.o,$(SRCSCPP))

all: $(LIBOUT) $(LIBCRYPTO)

$(OBJSC): $(LIBOUT)/%.o : %.c
	$(GCC) -c $< -o $@

$(OBJSCPP): $(LIBOUT)/%.o : %.cpp
	echo $(GXX) 
	$(GXX) -c $< -o $@

$(LIBCRYPTO): $(OBJSCPP)
	$(AR) $(ARFLAGS) $@ $(OBJSC) $(OBJSCPP)
	echo 111142
$(LIBOUT):
	mkdir -p $(LIBOUT)/libSBRdec/src $(LIBOUT)/libMpegTPEnc/src $(LIBOUT)/libAACenc/src $(LIBOUT)/libPCMutils/src \
	$(LIBOUT)/libSBRenc/src $(LIBOUT)/libMpegTPDec/src $(LIBOUT)/libAACdec/src $(LIBOUT)/libFDK/src $(LIBOUT)/libFDK/src/arm \
	$(LIBOUT)/libSYS/src

clean:
	@$(RM) $(LIBOUT) $(LIBCRYPTO) *.bak *~

.PHONY: all clean
