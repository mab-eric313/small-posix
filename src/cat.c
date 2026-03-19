#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char **argv) {
	FILE *fp;
	char buffer[BUFFER_SIZE];
	size_t bytes_read;

	if (argc < 2) {
		fprintf(stderr, "Usage: simple-cat [FILE]...\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 1; i < argc; i++) {
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "Error opening %s for reading\n", argv[i]);
			exit(EXIT_FAILURE);
		}
		while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
			fwrite(buffer, 1, bytes_read, stdout);
		fclose(fp);
	}

	return 0;
}
