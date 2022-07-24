#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strcmp, strlen */
#ifdef _MSC_VER
# include <io.h> /* for _access */
#else
# include <unistd.h> /* for access */
#endif


static char *_read_stdin(void) {
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
		if (text[len - 2] == '\r') {
			text[len - 2] = '\0';
		}
		text[len - 1] = '\0';
	}
	return text;
}

char *read_file(const char *path) {
	if (strcmp(path, "-") == 0) {
		return _read_stdin();
	}
	FILE *file = fopen(path, "rb");
	if (file == NULL) {
		return NULL;
	}
	if (fseek(file, 0, SEEK_END) < 0) {
		fclose(file);
		return NULL;
	}
	long pos = ftell(file);
	if (pos < 0) {
		fclose(file);
		return NULL;
	}
	unsigned long filesize = pos;
	char *text = malloc(filesize + 1);
	if (text == NULL) {
		fclose(file);
		return NULL;
	}
	rewind(file);
	bool file_err = fread(text, 1, filesize, file) < filesize || ferror(file);
	fclose(file);
	if (file_err) {
		free(text);
		return NULL;
	}
	if (text[filesize - 1] == '\n') {
		if (text[filesize - 2] == '\r') {
			text[filesize - 2] = '\0';
			/* Try to free the unused byte(s). Do nothing if realloc fails,
			   since the old pointer is not changed. But since we actually free
			   memory it shouldn't actually fail */
			char *tmp = realloc(text, filesize - 1);
			if (tmp != NULL) {
				text = tmp;
			}
		} else {
			text[filesize - 1] = '\0';
			char *tmp = realloc(text, filesize);
			if (tmp != NULL) {
				text = tmp;
			}
		}
	} else {
		text[filesize] = '\0';
	}
	return text;
}


static int _write_stdout(const char *text) {
	size_t len = strlen(text);
	if (fwrite(text, 1, len, stdout) < len) {
		return -__LINE__;
	}
	if (text[len - 1] != '\n' && fputc('\n', stdout) != '\n') {
		return -__LINE__;
	}
	return 0;
}

int write_file(const char *path, const char *text) {
	if (strcmp(path, "-") == 0) {
		return _write_stdout(text);
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

bool is_file(const char *path) {
#ifdef _MSC_VER
	return _access(path, 0) == 0;
#else
	return access(path, F_OK) == 0;
#endif
}
