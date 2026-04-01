#define _POSIX_C_SOURCE 200809L
// #define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define PATHNAME_SIZE 1024

int main(int argc, char **argv) {
	if (argc <= 1) {
		fprintf(stderr, "usage: touch [FILE...]\n");
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		struct stat st_file;
		if (stat(argv[i], &st_file) < 0) {
			int fd = open(argv[i], O_CREAT|O_WRONLY);
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
