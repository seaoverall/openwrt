#onvif ¹¤³ÌmakefileÄ¿Â¼
PROJDIR = .
TOPDIR  = $(PROJDIR)/..
OBJSDIR = $(PROJDIR)/objects
INCSDIR = $(PROJDIR)/include 
SRCS  += $(wildcard $(PROJDIR)/*.c)
OBJS  := $(addprefix $(OBJSDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))

CC = gcc -DDEBUG -DWITH_OPENSSL

#CFLAGS  += -g -O2
CFLAGS  += -O2 
CFLAGS  += -I./include/libavformat -I./include/libavcodec -I./include/libavutil -I. -I../openssl -I./include
#-I../openssl  -I./include/libavutil
#FFMPEGINC := -I$(TOPDIR)/ffmpeg/include/libavcodec \
			 -I$(TOPDIR)/ffmpeg/include/libavdevice \
			 -I$(TOPDIR)/ffmpeg/include/libavfilter \
			 -I$(TOPDIR)/ffmpeg/include/libavformat \
			 -I$(TOPDIR)/ffmpeg/include/libavutil \
			 -I$(TOPDIR)/ffmpeg/include/libswresample \
#			 -I$(TOPDIR)/ffmpeg/include/libswscale
			 
#FFMPEGLIB := -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale

#CFLAGS += $(FFMPEGINC) 
RTSPLIB := -lrtspcli
LDFLAGS += -ldl -lm -lpthread -llzma
EXENAME = onvif-client 
DEPFILE = deps

.PHONY:all
all: $(DEPFILE)  $(EXENAME) 

$(DEPFILE): $(SRCS) Makefile
	@-rm -f $(DEPFILE)
	@for f in $(SRCS); do \
		OBJ=$(OBJSDIR)/`basename $$f|sed  -e 's/\.c/\.o/'`; \
		echo $$OBJ: $$f >> $(DEPFILE); \
		echo '	$(CC) $$(CFLAGS) -c -o $$@ $$^'>> $(DEPFILE); \
		done

-include $(DEPFILE)
$(EXENAME): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o "$@" -L../lib -Wl,--start-group -lcrypto -lssl -lrtspcli $(RTSPLIB) -Wl,--end-group $(LDFLAGS)


.PHONY:clean
clean:
	rm -fr $(OBJSDIR)/*.o
	rm -fr $(DEPFILE)
	rm -fr $(EXENAME)
