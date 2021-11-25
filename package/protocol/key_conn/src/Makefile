#onvif ¹¤³ÌmakefileÄ¿Â¼
PROJDIR = .
OBJSDIR = $(PROJDIR)/objects
INCSDIR = $(PROJDIR)/include

SRCS  += $(wildcard $(PROJDIR)/*.c)
OBJS  := $(addprefix $(OBJSDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))

CC = gcc -DDEBUG -DWITH_OPENSSL

CFLAGS  += -O2 -fPIC -DWITH_NOIDREF 
CFLAGS  += -I./include/libavformat -I./include/libavcodec -I./include/libavutil -I. -I./include

LDFLAGS += -ldl -lm -lpthread -llzma
EXENAME = onvif
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
	$(CC) $(CFLAGS)  $(OBJS) -o "$@" -Wl,--start-group -lcrypto -lssl -Wl,--end-group $(LDFLAGS)


.PHONY:clean
clean:
	rm -fr $(OBJSDIR)/*.o
	rm -fr $(DEPFILE)
	rm -fr $(EXENAME)
