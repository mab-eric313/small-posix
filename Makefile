CC := cc
CFLAGS := -std=c99 -Wall -Wextra
TARGET := cat pwd echo ls mkdir rmdir

all: $(TARGET)

cat: ./src/cat.c
	$(CC) $(CFLAGS) -o $@ $<

pwd: ./src/pwd.c
	$(CC) $(CFLAGS) -o $@ $<

echo: ./src/echo.c
	$(CC) $(CFLAGS) -o $@ $<

ls: ./src/ls.c
	$(CC) $(CFLAGS) -o $@ $<

mkdir: ./src/mkdir.c
	$(CC) $(CFLAGS) -o $@ $<

rmdir: ./src/rmdir.c
	$(CC) $(CFLAGS) -o $@ $<

touch: ./src/touch.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
