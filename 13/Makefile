all: main.o
	$(CC) $^
%.o: %.c
	$(CC) -o $@ -c $<

.PHONY: clean
clean:
	$(RM) *.o *~ a.out
