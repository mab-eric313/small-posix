#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <stdlib.h>

void help(char **argv, int status) {
	printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);

	if (status >= 0) {
		fputs("List information about the FILE(s) (the current "
				"directory by default)", stdout);
		fputs("\n\
\n  Options:\
\n  -h 		display this help and exit\
\n", stdout);
	}

	exit(status);
}

int list(const char *path) {
	struct dirent *entry;
	DIR *dp = opendir(path); // TODO: use open() instead opendir()
	
	if (dp == NULL) {
		perror("opendir");
		return -1;
	}

	while ((entry = readdir(dp))) {
		printf("%s", entry->d_name);
		if (dp != NULL)
			printf("  ");
	}
	putchar('\n');

	closedir(dp);

	return 0;
}

int main(int argc, char **argv) {
	int counter = 0;

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
	
	if (argc == 1)
		list(".");
	
	while (counter++ < argc-1) {
		printf("%s:\n", argv[counter]);
		list(argv[counter]);
		if (counter < argc-1)
			putchar('\n');
	}

	return 0;
}
