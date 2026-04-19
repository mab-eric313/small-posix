CC := musl-gcc
CFLAGS := -std=c99 -Wall -Wextra
TARGET := cat pwd echo ls mkdir rmdir touch cp rm

ifdef debug
	CFLAGS += -g
endif

ifdef static
	CFLAGS += -static
	MSG += "Building with static link\n"
endif

ifdef m32
	CFLAGS += -m32
	ARCH_MSG += "Building for 32-bit platform..."
else
	CFLAGS += -m64
	ARCH_MSG += "Building for 64-bit platform..."
endif

all: info $(TARGET)

info: 
	@echo $(MSG) $(ARCH_MSG)

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

cp: ./src/cp.c
	$(CC) $(CFLAGS) -o $@ $<

rm: ./src/rm.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
