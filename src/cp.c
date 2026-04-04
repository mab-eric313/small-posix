#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int is_regular_file(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) < 0) {
		return -1;
	}
    return S_ISREG(path_stat.st_mode);
}

int is_exist(const char *path) {
	if (is_regular_file(path) == 1) { // 'path' is exist and is a file
		return 1;
	} else if (is_regular_file(path) == 0) { // 'path' is directory
		return 0;
	} else { // 'path' doesn't exist
		return -1;
	}
}

int main(int argc, char **argv) {
	char *buffer[BUFFER_SIZE];
	int src_fd, dst_fd, dst_dirfd;
	src_fd = dst_fd = dst_dirfd = -1;

	if (argc <= 2) {
		fprintf(stderr, "Usage: cp [SOURCES...] [DEST FILES...]\n");
		return -1;
	}

	for (int i = 1; i < argc-1; i++) {
		struct stat st_file;
		int src_st_mode;

		if (stat(argv[i], &st_file) < 0) {
			perror("stat() error");
			return -1;
		}

		if (is_exist(argv[argc-1]) == 0) {
			dst_dirfd = open(argv[argc-1], O_RDONLY | O_DIRECTORY);

		// TODO: add confirmation, if there is a file in 'path', can i overwrite it?
			dst_fd = openat(dst_dirfd, argv[i], O_TRUNC |O_WRONLY | O_CREAT, 0666);
		} else if (is_exist(argv[argc-1]) == -1) {
			dst_fd = open(argv[argc-1], O_WRONLY | O_CREAT, 0666);
		}

		if (dst_fd < 0) {
			perror("open() destination file error");
			return -1;
		}

		src_fd = open(argv[i], O_RDONLY);
		if (src_fd < 0) {
			perror("open() source file error");
			return -1;
		}

		int ret;
		while ((ret = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
			buffer[ret] = 0x00;
			if (write(dst_fd, buffer, ret) != ret) {
				perror("write() error");
				return -1;
			}
		}

		src_st_mode = st_file.st_mode;
		// TODO: remove fchmod(), use open() instead
		if (fchmod(dst_fd, src_st_mode) < 0) {
			perror("fchmod() error");
			return -1;
		}
		
		close(src_fd);
		close(dst_dirfd);
		close(dst_fd);
	}

	return 0;
}
