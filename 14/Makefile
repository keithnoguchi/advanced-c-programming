TARGET := a.out
all: $(TARGET)
$(TARGET): main.o
	$(CC) -o $@ $^
%.o: %.c
	$(CC) -c -o $@ $<
.PHONY: clean
clean:
	$(RM) *.o *~ btree $(TARGET)
