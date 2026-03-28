#include <stdio.h>
#include <unistd.h>

#define PATHNAME_SIZE 1024

int main(int argc, char **argv) {
	char *pathname[PATHNAME_SIZE];

	if (argc <= 1) {
		fprintf(stderr, "usage: rmdir [DIRECTORY...]\n");
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		pathname[i] = argv[i];
		if (rmdir(pathname[i]) == -1) {
			perror("error: rmdir failed to remove a directory");
			return -1;
		}
	}

	return 0;
}

