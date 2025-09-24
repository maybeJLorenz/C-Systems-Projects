CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic -gdwarf-4
LFLAGS = 

HEADERS = bitreader.h bitwriter.h node.h pq.h


.PHONY: clean format scan-build

all: brtest bwtest nodetest pqtest huff dehuff


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

brtest: bitreader.o brtest.o
	$(CC) $^ $(LFLAGS) -o $@

bwtest: bitwriter.o bwtest.o
	$(CC) $^ $(LFLAGS) -o $@

nodetest: nodetest.o node.o
	$(CC) $^ $(LFLAGS) -o $@

pqtest: pqtest.o pq.o node.o
	$(CC) $^ $(LFLAGS) -o $@

huff: bitreader.o bitwriter.o pq.o node.o huff.o
	$(CC) $^ $(LFLAGS) -o $@

dehuff: bitreader.o bitwriter.o pq.o node.o dehuff.o
	$(CC) $^ $(LFLAGS) -o $@

clean:
	rm -f *.o brtest bwtest nodetest pqtest huff dehuff

scan-build: clean
	scan-build --use-cc=clang make

format:
	clang-format -i -style=file *.[ch]

