#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <libgen.h>
#include <stdbool.h>

typedef struct {
	bool recursive;
} Options;

void options_init(Options *o) {
	o->recursive = false;
}

void diagnose_leading_hypen(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		struct stat st;
		char *arg = argv[i];

		if (arg[0] == '-' && arg[1] && lstat(arg, &st) == 0) {
			fprintf(stderr, "error: incorrent option %c\n", arg[1]);
			break;
		}
	}
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		fprintf(stderr, "usage: rm [FILES...]\n");
		return -1;
	}

	int getopt_ret;
	Options opt;

	options_init(&opt);

	while ((getopt_ret = getopt(argc, argv, "r")) != -1) {
		switch(getopt_ret) {
			case 'r':
				opt.recursive = true;
				break;
			default:
				diagnose_leading_hypen(argc, argv); // TODO: Do i use this?
				fprintf(stderr, "error: incorrect options\n");
		}
	}

	// TODO: remove for-loop
	for (int i = optind; i < argc; i++) {
		struct stat st;
		int is_dir = (stat(argv[argc-1], &st) == 0 && S_ISDIR(st.st_mode));

		// TODO: --recursively: remove dir and the content inside
		// int dirfd = open(argv[i], O_DIRECTORY);
		// if (opt.recursive) {
		// 	if (unlinkat(dirfd, argv[i], 0) < 0) {
		// 		perror("unlinkat() error");
		// 	}
		// 	continue;
		// }

		if (is_dir) {
			if (rmdir(argv[i]) < 0) {
				perror("rmdir() error");
			} 
		} else {
			if (unlink(argv[i]) < 0) {
				perror("unlink() error");
				continue;
			}
		}
	}

	return 0;
}
