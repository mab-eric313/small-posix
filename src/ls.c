#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ENTRIES 1024

typedef struct {
	bool almost_all;
	bool all;
} Options;

void options_init(Options *o) {
	o->almost_all = false;
	o->all = false;
}

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("List information about the FILE(s) (the current "
				"directory by default)", stdout);
		fputs("\n\
\n  Options:\
\n  -A		listing all directory entries, including names begin with . \
\n			except . and ..\
\n  -a		listing all directory entries, including names begin with . \
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int compare(const void *a, const void *b) {
	return strcmp(*(const char **)a, *(const char **)b);
}

int list(const char *path, const char opt) {
	struct dirent *entry;
	DIR *dp = opendir(path);
	char *entries[MAX_ENTRIES];

	if (dp == NULL) {
		perror("opendir()");
		return -1;
	}

	int idx = 0;
	if (opt == '0') {
		while ((entry = readdir(dp)) != NULL && idx < MAX_ENTRIES) {
			if (entry->d_name[0] == '.')
				continue;
			entries[idx] = strdup(entry->d_name);
			++idx;
		}
	} else if (opt == 'A') {
		while ((entry = readdir(dp)) != NULL && idx < MAX_ENTRIES) {
			entries[idx] = strdup(entry->d_name);
			// TODO: memory leaks. do not use strcmp()
			if (strcmp(entries[idx], ".") == 0 || strcmp(entries[idx], "..") == 0)
				continue;
			++idx;
		}
	} else if (opt == 'a') {
		while ((entry = readdir(dp)) != NULL && idx < MAX_ENTRIES) {
			entries[idx] = strdup(entry->d_name);
			++idx;
		}
	}

	qsort(entries, idx, sizeof(char *), compare);
	
	for (int i = 0; i < idx; i++) {
		printf("%s  ", entries[i]);
		free(entries[i]);
	}
	putchar('\n');

	closedir(dp);
	return 0;
}

int main(int argc, char **argv) {
	int getopt_ret;
	Options opt;

	options_init(&opt);

	while ((getopt_ret = getopt(argc, argv, "Aah")) != -1) {
		switch(getopt_ret) {
			case 'A':
				opt.almost_all = true;
				break;
			case 'a':
				opt.all = true;
				break;
			case 'h':
				help(argv, 0);
				break;
			default:
				fprintf(stderr, "error: incorrect option\n");
		}
	}
	
	if (argc == 1) {
		list(".", '0');
		return 0;
	} else if (argc == 2 && opt.almost_all) {
		list(".", 'A');
		return 0;
	} else if (argc == 2 && opt.all) {
		list(".", 'a');
		return 0;
	}
	
	for (int i = optind; i < argc; i++) {
		if (opt.all)
			list(argv[i], 'a');
		else if (opt.almost_all)
			list(argv[i], 'A');
		else
			list(argv[i], '0');

		if (i < argc-1)
			putchar('\n');
	}

	return 0;
}
