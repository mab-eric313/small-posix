CC := cc
CFLAGS := -std=c99 -Wall -Wextra
TARGET := cat
SRC := ./src/cat.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
