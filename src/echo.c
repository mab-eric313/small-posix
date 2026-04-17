#include <stdio.h>

void echo(char **s) {
	static int first = 1;
	if (!first) {
		fputc(' ', stdout);
	}
	fputs(*s, stdout);
	first = 0;
}

int main(int argc, char **argv) {
	while (--argc >= 1)
		echo(++argv);

	putchar('\n');

	return 0;
}
