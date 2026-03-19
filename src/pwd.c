#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

int main(void) {
	char cwd[PATH_MAX];
	if (getcwd(cwd, 1024) != NULL) {
		printf("%s\n", cwd);
	} else {
		perror("error: cannot get the current working directory");
		return 1;
	}
	return 0;
}
