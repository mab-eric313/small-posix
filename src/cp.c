#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <linux/limits.h>

#define BUFFER_SIZE 4096

int main(int argc, char **argv) {
	if (argc <= 2) {
		fprintf(stderr, "Usage: %s [SOURCES...] [DEST FILES...]\n", argv[0]);
		return -1;
	}

	struct stat st_dest;
	int dest_is_dir = (stat(argv[argc-1], &st_dest) == 0 && 
						S_ISDIR(st_dest.st_mode));

	for (int i = 1; i < argc-1; i++) {
		struct stat st_src;
		char path_buffer[PATH_MAX];
		char *dest_path;

		if (stat(argv[i], &st_src) < 0) {
			perror("stat() source file error");
			continue;
		}

		if (dest_is_dir) {
			char *base = basename(argv[i]);
			snprintf(path_buffer, sizeof(path_buffer), "%s/%s", argv[argc-1], base);
			dest_path = path_buffer;
		} else {
			dest_path = argv[argc-1];
		}

		int src_fd = open(argv[i], O_RDONLY);
		// TODO: add confirmation, if there is a file in 'path', can i overwrite it?
		int dst_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, st_src.st_mode);

		if (src_fd < 0 || dst_fd < 0) {
			perror("open() error");
			if (src_fd >= 0) close(src_fd);
			if (dst_fd >= 0) close(dst_fd);
			continue;
		}

		char buffer[BUFFER_SIZE];
		ssize_t n_read;
		while ((n_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
			if (write(dst_fd, buffer, n_read) != n_read) {
				perror("write() error");
				break;
			}
		}

		if (n_read < 0) {
			perror("read() read source file error");
		}

		close(src_fd);
		close(dst_fd);
	}

	return 0;
}
