TARGET := a.out
SRC := main.c
OBJ := $(SRC:.c=.o)
TMP := *~ *.sw*
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean
clean:
	$(RM) $(OBJ) $(TARGET) $(TMP)
