CFLAGS=-Wall

PROG=date


.phony: client server $(PROG).h
.precious: $(PROG)_clnt.c $(PROG)_svc.c

all: client server

%.o: %.c $(DEPS)
	$(CC) $(CLFAGS) -c -o $@ $<

$(PROG).h: $(PROG).x
	rpcgen $<

%_svc.c: %.x
	rpcgen $<

%_clnt.c: %.x
	rpcgen $<

client: $(PROG).h $(PROG)_client.o $(PROG)_clnt.o
	$(CC) $(CFLAGS) -o $@ $(PROG)_client.o $(PROG)_clnt.o

server: $(PROG).h $(PROG)_server.o $(PROG)_svc.o
	$(CC) $(CFLAGS) -o $@ $(PROG)_server.o $(PROG)_svc.o

clean:
	rm -f client server *.o $(PROG)_svc.c $(PROG)_clnt.c $(PROG).h
