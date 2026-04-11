#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

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

int concat(int fd1, int fd2) {
	char buf[BUFSIZ];
	ssize_t bytes_read;

	while ((bytes_read = read(fd1, buf, sizeof(buf))) > 0) {
		if (write(fd2, buf, bytes_read) < 0) {
			perror("write() error");
			return -2;
		}
	}

	if (bytes_read < 0) {
		perror("read() error");
		return -1;
	}

	return 0;
}

int main(int argc, char **argv) {
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
		struct stat st;
		if (stat(argv[i], &st) < 0) {
			perror("stat() error");
			continue;
		}
		int fd = open(argv[i], O_RDONLY);
		if (fd < 0) {
			perror("open() error");
			continue;
		}

		if (concat(fd, 1) < 0) continue;

		if (fd != 0) close(fd);
	}

	return 0;
}
