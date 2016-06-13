CFLAGS += -g
all: a.out
a.out: main.o
	$(CC) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o *~ a.out
