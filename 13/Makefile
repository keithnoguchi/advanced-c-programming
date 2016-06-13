CFLAGS += -g
all: bintree
bintree: main.o
	$(CC) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o *~ a.out bintree
