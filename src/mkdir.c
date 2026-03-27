#include <stdio.h>
#include <sys/stat.h>

#define PATHNAME_SIZE 1024

int main(int argc, char **argv) {
	char *pathname[PATHNAME_SIZE];

	int i = 0;
	if (argc > 1)
		while (++i < argc) {
			printf("argv[i]: %s\n", argv[i]);
			pathname[i] = argv[i];
			if (mkdir(pathname[i], 0777) == -1) {
				perror("error: mkdir failed to create a dir\n");
				return -1;
			}
		}
	else {
		perror("Usage: mkdir [pathname]\n");
		return -1;
	}

	return 0;
}
