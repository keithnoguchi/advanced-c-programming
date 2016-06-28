TARGET := a.out
SRC := main.c
OBJ := $(SRC:.c=.o)
TMP := *~ *.swp
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ 
%.o: %.c
	$(CC) -o $@ -c $<
.PHONY: clean
clean:
	$(RM) $(OBJ) $(TARGET) $(TMP)
