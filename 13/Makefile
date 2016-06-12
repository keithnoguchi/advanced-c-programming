CFLAGS += -g
all: main.o
	$(CC) $^
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o *~ a.out
