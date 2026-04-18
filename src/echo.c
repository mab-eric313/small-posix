#include <stdio.h>
#include <string.h>

void echo(char *s) {
	static int first = 1;
	if (!first) {
		fputc(' ', stdout);
	}
	fputs(s, stdout);
	first = 0;
}

int main(int argc, char **argv) {
	int nflag = 0;

	if (strcmp(argv[1], "-n") == 0) {
		nflag = 1;
		argc -= 2;
		argv += 2;
	} else {
		argc--, argv++;
	}

	for (; *argv; argc--, argv++)
		echo(*argv);

	if (nflag == 0)
		putchar('\n');

	return 0;
}
