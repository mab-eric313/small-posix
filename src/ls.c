#include <stdio.h>
#include <dirent.h>

int list(const char *path) {
	struct dirent *entry;
	DIR *dp = opendir(path);
	
	if (dp == NULL) {
		perror("opendir");
		return -1;
	}

	while ((entry = readdir(dp))) {
		printf("%s", entry->d_name);
		if (dp != NULL)
			putchar(' ');
	}
	putchar('\n');

	closedir(dp);

	return 0;
}

int main(int argc, char **argv) {
	int counter = 0;
	
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
