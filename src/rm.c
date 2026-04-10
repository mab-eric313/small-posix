#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	bool recursive;
} Options;

void options_init(Options *o) {
	o->recursive = false;
}

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("Remove (unlink) the FILE(s).", stdout);
		fputs("\n\
\n  Options:\
\n  -r		remove directories and their contents recursively\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
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

void recursive_rm(const char *path) {
	DIR *d = opendir(path);
	struct dirent *entry;

	if (!d) {
		perror("opendir() error");
		return;
    }

	while ((entry = readdir(d)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

		if (entry->d_type == 4) { // DT_DIR = 4
			recursive_rm(full_path);
		} else {
			unlink(full_path);
		}
	}
	closedir(d);

    rmdir(path);
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		help(argv, -1);
	}

	int getopt_ret;
	Options opt;

	options_init(&opt);

	while ((getopt_ret = getopt(argc, argv, "rh")) != -1) {
		switch(getopt_ret) {
			case 'R':
			case 'r':
				opt.recursive = true;
				break;
			case 'h':
				help(argv, 0);
				break;
			default:
				diagnose_leading_hypen(argc, argv); // TODO: should i use this?
				fprintf(stderr, "error: incorrect options\n");
		}
	}

	// TODO: remove for-loop
	for (int i = optind; i < argc; i++) {
		struct stat st;
		int is_dir = (stat(argv[argc-1], &st) == 0 && S_ISDIR(st.st_mode));

		if (opt.recursive)
			recursive_rm(argv[i]);

		else if (is_dir) {
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
