#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp, strlen */
#include <unistd.h> /* for access */



char *readFile(const char *path) {
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		return NULL;
	}
	if (fseek(file, 0, SEEK_END) < 0) {
		fclose(file);
		return NULL;
	}
	long filesize = ftell(file);
	if (filesize < 0) {
		fclose(file);
		return NULL;
	}
	char *text = malloc(filesize + 1);
	if (text == NULL) {
		fclose(file);
		return NULL;
	}
	rewind(file);
	if (fread(text, 1, filesize, file) < (unsigned) filesize || ferror(file)) {
		free(text);
		fclose(file);
		return NULL;
	}
	if (text[filesize - 1] == '\n') {
		text[filesize - 1] = '\0';
	} else {
		text[filesize] = '\0';
	}
	fclose(file);
	return text;
}


static int writeStdout(const char *text) {
	size_t len = strlen(text);
	if (fwrite(text, 1, len, stdout) < len) {
		return -1;
	}
	if (text[len - 1] != '\n' && fputc('\n', stdout) != '\n') {
		return -2;
	}
	return 0;
}

int writeFile(const char *path, const char *text) {
	if (strcmp(path, "-") == 0) {
		return writeStdout(text);
	}
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		return -1;
	}
	size_t len = strlen(text);
	if (fwrite(text, 1, len, file) < len) {
		fclose(file);
		return -2;
	}
	if (text[len - 1] != '\n' && fputc('\n', file) != '\n') {
		fclose(file);
		return -3;
	}
	fclose(file);
	return 0;
}

bool isFile(const char *path) {
	return access(path, F_OK) == 0;
}
