#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

// typedef struct {
// } Options;
// 
// void options_init(Options *o) {
// }

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("Concatenate FILE(s) to standard output", stdout);
		fputs("\n\
\n  Options:\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int main(int argc, char **argv) {
	// TODO: change from file pointer to file descriptor
	FILE *fp;
	char buffer[BUFFER_SIZE];
	size_t bytes_read;

	if (argc <= 1) help(argv, -1);

	int getopt_ret;
	// Options opt;

	// options_init(&opt);

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
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			perror("fopen() error");
			continue;
		}
		while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
			fwrite(buffer, 1, bytes_read, stdout);
		fclose(fp);
	}

	return 0;
}
