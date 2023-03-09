#include "include/file.h"

bool is_folder(const char *folder_path) {
	struct stat sb;
	return ((stat(folder_path, &sb) == 0) && S_ISDIR(sb.st_mode));
}

bool is_file(const char *file_path) {
	// Check if this file exists. A return value of 0 means yes, so we not it.
	return !access(file_path, F_OK);
}

const char *read_entire_file(const char *file_path) {
	if (!is_file(file_path)) {
		fprintf(stderr,
		        "[ERROR] File \"%s\" does not exist in the filesystem!\n",
		        file_path);
		return NULL;
	}
	FILE *file;

	file = fopen(file_path, "r");
	if (!file) {
		fprintf(
			stderr,
			"[ERROR] File \"%s\" exists but cannot be opened (check permissions).\n",
			file_path);
		return NULL;
	}

	size_t file_size = size_file(file);

	void *file_contents = calloc(file_size + 1UL, sizeof(char));
	if (!file_contents) {
		fprintf(stderr, "[ERROR] Memory allocation failure.\n");
		fclose(file);
		return NULL;
	}

	fseek(file, 0L, SEEK_SET);
	fread(file_contents, sizeof(char), file_size, file);
	fclose(file);

	return (const char *)file_contents;
}

size_t size_file(FILE *file) {
	fseek(file, 0L, SEEK_END);
	return (size_t)ftell(file);
}
