#include <stdio.h>
#include <sys/stat.h>

#define PATHNAME_SIZE 1024

int main(int argc, char **argv) {
	char *pathname[PATHNAME_SIZE];

	if (argc <= 1) {
		fprintf(stderr, "usage: mkdir [DIRECTORY...]\n");
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		pathname[i] = argv[i];
		if (mkdir(pathname[i], 0777) == -1) {
			perror("error: mkdir failed to create a dir");
			return -1;
		}
	}

	return 0;
}
