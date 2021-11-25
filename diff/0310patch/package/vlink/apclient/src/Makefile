EXEC = ap_client ap_client_stop

all: $(EXEC)
      
ap_client: ap_client.o
	$(CC) $(LDFLAGS) ap_client.o -o ap_client
      
ap_client.o: ap_client.c
	$(CC) $(CFLAGS) -c ap_client.c

ap_client_stop: ap_client_stop.o
	$(CC) $(LDFLAGS) ap_client_stop.o -o ap_client_stop
      
ap_client_stop.o: ap_client_stop.c
	$(CC) $(CFLAGS) -c ap_client_stop.c

      
# remove object files and executable when user executes "make clean"  
clean:  
	rm *.o ap_client ap_client_stop
