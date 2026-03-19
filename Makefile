CC := cc
CFLAGS := -std=c99 -Wall -Wextra

all: cat pwd

cat: ./src/cat.c
	$(CC) $(CFLAGS) -o $@ $<

pwd: ./src/pwd.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
