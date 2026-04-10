#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <getopt.h>

#define BUFFER_SIZE 4096

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [SOURCE]... [DEST]...\n", argv[0]);

	if (status >= 0) {
		fputs("Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY", stdout);
		fputs("\n\
\n  Options:\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		help(argv, -1);
	}

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

	struct stat st_dest;
	int dest_is_dir = (stat(argv[argc-1], &st_dest) == 0 && 
						S_ISDIR(st_dest.st_mode));

	for (int i = optind; i < argc-1; i++) {
		struct stat st_src;
		char path_buffer[PATH_MAX];
		char *dest_path;

		if (stat(argv[i], &st_src) < 0) {
			perror("stat() source file error");
			continue;
		}

		char *src_base = basename(argv[i]);

		if (dest_is_dir) {
			snprintf(path_buffer, sizeof(path_buffer), "%s/%s", argv[argc-1],
					src_base);
			dest_path = path_buffer;
		} else {
			dest_path = argv[argc-1];
		}

		struct stat st_target;
		int target_exists = (stat(dest_path, &st_target) == 0);

		if (target_exists && st_src.st_dev == st_target.st_dev && 
				st_src.st_ino == st_target.st_ino) {
			fprintf(stderr, "%s: '%s' and '%s' are the same file\n", 
					argv[0], argv[i], dest_path);
			continue;
		}

		if (target_exists) {
			char conf[10];
			printf("%s: overwrite '%s' (y/n)? ", argv[0], dest_path);

			if (!fgets(conf, sizeof(conf), stdin) || 
					(conf[0] != 'y' && conf[0] != 'Y')) {
				printf("skipped\n");
				continue;
			}
		}

		int src_fd = open(argv[i], O_RDONLY);
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
