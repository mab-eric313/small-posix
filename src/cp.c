#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char **argv) {
	char *buffer[BUFFER_SIZE];
	int src_fd, dst_fd;

	if (argc <= 2) {
		fprintf(stderr, "Usage: cp [SOURCES...] [DEST FILES...]\n");
		return -1;
	}

	dst_fd = open(argv[argc-1], O_WRONLY | O_CREAT, 0666);
	if (dst_fd < 0) {
		perror("open() destination file error");
		return -1;
	}

	for (int i = 1; i < argc-1; i++) {
		struct stat st_file;
		int src_st_mode;

		if (stat(argv[i], &st_file) < 0) {
			perror("stat() error");
			return -1;
		}
		src_fd = open(argv[i], O_RDONLY);
		if (src_fd < 0) {
			perror("open() source file error");
			return -1;
		}

		src_st_mode = st_file.st_mode;

		int ret;
		while ((ret = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
			buffer[ret] = 0x00;
			if (write(dst_fd, buffer, ret) != ret) {
				perror("write() error");
				return -1;
			}
		}

		// TODO: remove fchmod(), use open() instead
		if (fchmod(dst_fd, src_st_mode) < 0) {
			perror("fchmod() error");
			return -1;
		}

		close(src_fd);
		close(dst_fd);
	}

	return 0;
}
