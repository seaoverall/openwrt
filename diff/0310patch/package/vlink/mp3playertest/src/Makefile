EXEC = mp3playertest wavplayertest
# build mp3playertest executable when user executes "make"  
      
all: $(EXEC)

mp3playertest: mp3playertest.o  
	$(CC) $(LDFLAGS) mp3playertest.o -o mp3playertest -lmad -lasound
      
mp3playertest.o: mp3playertest.c  
	$(CC) $(CFLAGS) -c mp3playertest.c -lmad -lasound

wavplayertest: wavplayertest.o  
	$(CC) $(LDFLAGS) wavplayertest.o -o wavplayertest -lmad -lasound
      
wavplayertest.o: wavplayertest.c  
	$(CC) $(CFLAGS) -c wavplayertest.c -lmad -lasound
      
# remove object files and executable when user executes "make clean"  
clean:  
	rm *.o mp3playertest wavplayertest  
