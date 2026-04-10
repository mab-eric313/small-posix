#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#define PATHNAME_SIZE 1024

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [DIR]...\n", argv[0]);

	if (status >= 0) {
		fputs("Remove the DIRECTORY(ies), if they are empty.", stdout);
		fputs("\n\
\n  Options:\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int main(int argc, char **argv) {
	char *pathname[PATHNAME_SIZE];

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
		pathname[i] = argv[i];
		if (rmdir(pathname[i]) == -1) {
			perror("error: rmdir failed to remove a directory");
			return -1;
		}
	}

	return 0;
}

