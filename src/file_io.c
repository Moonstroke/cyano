#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp, strlen */
#include <unistd.h> /* for access */



static char *_readStdin(void) {
	/* Since stdin is a stream and not a regular file, fseek won't work on it.
	   The only way is to iterate with a buffer and append to the result
	   string */
	char *text = NULL;
	char buffer[128];
	size_t len = 0;
	size_t read;
	while ((read = fread(buffer, 1, sizeof buffer, stdin)) > 0) {
		text = realloc(text, len + read);
		if (text == NULL) {
			return NULL;
		}
		strncpy(text + len, buffer, read);
		len += read;
	}
	if (ferror(stdin)) {
		free(text);
		return NULL;
	}
	if (text[len - 1] == '\n') {
		text[len - 1] = '\0';
	}
	return text;
}

char *readFile(const char *path) {
	if (strcmp(path, "-") == 0) {
		return _readStdin();
	}
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


static int _writeStdout(const char *text) {
	size_t len = strlen(text);
	if (fwrite(text, 1, len, stdout) < len) {
		return -__LINE__;
	}
	if (text[len - 1] != '\n' && fputc('\n', stdout) != '\n') {
		return -__LINE__;
	}
	return 0;
}

int writeFile(const char *path, const char *text) {
	if (strcmp(path, "-") == 0) {
		return _writeStdout(text);
	}
	FILE *file = fopen(path, "w");
	if (file == NULL) {
		return -__LINE__;
	}
	size_t len = strlen(text);
	if (fwrite(text, 1, len, file) < len) {
		fclose(file);
		return -__LINE__;
	}
	if (text[len - 1] != '\n' && fputc('\n', file) != '\n') {
		fclose(file);
		return -__LINE__;
	}
	fclose(file);
	return 0;
}

bool isFile(const char *path) {
	return access(path, F_OK) == 0;
}
