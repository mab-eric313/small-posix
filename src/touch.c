#define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>

#define PATHNAME_SIZE 1024

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("Update the access and modification times of "
				"each FILE to the current time.", stdout);
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
		struct stat st_file;
		if (stat(argv[i], &st_file) < 0) {
			int fd = open(argv[i], O_CREAT|O_WRONLY, 0666);
			if (fd < 0) {
				perror("open() error");
				return -1;
			}
			close(fd);
		}

		struct timespec ts[2];
		ts[0].tv_nsec = UTIME_NOW; // Access time
		ts[1].tv_nsec = UTIME_NOW; // Modification time
		
		if (utimensat(AT_FDCWD, argv[i], ts, 0) < 0) {
			perror("utimensat() error");
		}
	}

	return 0;
}
