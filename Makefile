CC := cc
CFLAGS := -std=c99 -Wall -Wextra
TARGET := cat pwd echo

all: $(TARGET)

cat: ./src/cat.c
	$(CC) $(CFLAGS) -o $@ $<

pwd: ./src/pwd.c
	$(CC) $(CFLAGS) -o $@ $<

echo: ./src/echo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
