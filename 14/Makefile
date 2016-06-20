CFLAGS += -g -Wall -Werror
LDFLAGS += -g
TARGET := a.out
all: $(TARGET)
$(TARGET): main.o
	$(CC) $(LDFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
.PHONY: clean
clean:
	$(RM) *.o *~ btree $(TARGET)
