#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>

#define PATHNAME_SIZE 1024

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("Create the DIRECTORY(ies), if they do not already exist", stdout);
		fputs("\n\
\n  Options:\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int main(int argc, char **argv) {
	if (argc <= 1) help(argv, -1);

	int getopt_ret;
	while ((getopt_ret = getopt(argc, argv, "h")) != -1) {
		switch(getopt_ret) {
			case 'h':
				help(argv, 0);
				break;
			default:
				fprintf(stderr, "error: incorrect option\n");
		}
	}

	for (int i = optind; i < argc; i++) {
		argv[i] = argv[i];
		if (mkdir(argv[i], 0777) == -1) {
			perror("error: mkdir failed to create a dir");
			return -1;
		}
	}

	return 0;
}
