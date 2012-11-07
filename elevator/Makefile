# Forrest Brewer 10/27/2009

CC  = gcc
RM = rm

# CFLAGS  = -g -c -Wall -o$@
CFLAGS  = -O -c -Wall  -o$@

qhsmtst: main.o qhsmtst.o qepn.o
	$(CC) -ohsmtst main.o qhsmtst.o qepn.o

qhsmtst.o: qhsmtst.c
	$(CC) $(CFLAGS) $<

main.o: main.c
	$(CC) $(CFLAGS) $<

qepn.o: qepn.c
	$(CC) $(CFLAGS) $<

clean:
	-$(RM) *.o hsmtst



