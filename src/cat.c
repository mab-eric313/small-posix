#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct {
	bool number;
} Options;

void options_init(Options *o) {
	o->number = false;
}

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("Concatenate FILE(s) to standard output", stdout);
		fputs("\n\
\n  Options:\
\n	-n		show number per line\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int concat(int fd1, int fd2, char opt) {
    char buf[BUFSIZ];
    ssize_t bytes_read;
    static int num = 1;
    int start_of_line = 1;

    while ((bytes_read = read(fd1, buf, sizeof(buf))) > 0) {
        if (opt == 'n') {
            for (ssize_t i = 0; i < bytes_read; i++) {
                if (start_of_line) {
                    char num_buf[32];
                    int n = snprintf(num_buf, sizeof(num_buf), "%6d  ", num++);
                    write(fd2, num_buf, n);
                    start_of_line = 0;
                }

                write(fd2, &buf[i], 1);

                if (buf[i] == '\n') {
                    start_of_line = 1;
                }
            }
        } else {
            if (write(fd2, buf, bytes_read) < 0) {
                perror("write() error");
                return -2;
            }
        }
    }

    if (bytes_read < 0) {
        perror("read() error");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
	int getopt_ret;
	Options opt;

	options_init(&opt);

	while ((getopt_ret = getopt(argc, argv, "nh")) != -1) {
		switch(getopt_ret) {
			case 'n':
				opt.number = true;
				break;
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

		if (opt.number) {
			if (concat(fd, 1, 'n') < 0) return -1;
		} else if (concat(fd, 1, '0') < 0) continue;

		if (fd != 0) close(fd);
	}

	return 0;
}
