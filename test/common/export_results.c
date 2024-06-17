/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2024 Nokia
 */

#include <odp/helper/odph_api.h>

#include "export_results.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/** Maximum length of caller function name */
#define MAX_FILE_NAME_LEN 64

/** Maximum length of current working directory for exporting */
#define MAX_EXPORT_CWD_LEN 128

/** Maximum length of path for exporting */
#define MAX_EXPORT_PATH_LEN 192
typedef struct {
	test_common_options_t common_options;

	char filename[MAX_FILE_NAME_LEN];

	char path[MAX_EXPORT_PATH_LEN];

	FILE *file;

} test_export_gbl_t;

static test_export_gbl_t gbl_data;

int test_common_parse_options(int argc, char *argv[])
{
	char *env, *last_slash;
	int i, j;

	env = getenv("TEST_COMMON_EXPORT");
	if (env) {
		gbl_data.common_options.is_export = true;
		odph_strcpy(gbl_data.path, env, MAX_EXPORT_PATH_LEN);
	}

	/* Find and remove option */
	for (i = 0; i < argc;) {
		if (strcmp(argv[i], "--test-common-export") == 0) {
			gbl_data.common_options.is_export = true;
			if (i + 1 < argc && argv[i + 1][0] != '-') {
				odph_strcpy(gbl_data.path, argv[i + 1],
					    MAX_EXPORT_PATH_LEN);
				for (j = i; j < argc - 2; j++)
					argv[j] = argv[j + 2];
				argc -= 2;
				continue;
			} else {
				last_slash = strrchr(argv[0], '/');
				if (last_slash)
					odph_strcpy(gbl_data.filename, last_slash + 1,
						    MAX_FILE_NAME_LEN);
				else
					odph_strcpy(gbl_data.filename, argv[0], MAX_FILE_NAME_LEN);
				for (j = i; j < argc - 1; j++)
					argv[j] = argv[j + 1];
				argc -= 1;
			}
		}
		i++;
	}
	return argc;
}

int test_common_options(test_common_options_t *options)
{
	char cwd[MAX_EXPORT_CWD_LEN];
	const char *new_ending = "_output.csv";
	int path_len;

	memset(options, 0, sizeof(*options));

	options->is_export = gbl_data.common_options.is_export;

	if (strlen(gbl_data.path) == 0) {
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			ODPH_ERR("Failed to get current working directory: %s\n", strerror(errno));
			return -1;
		}

		path_len = snprintf(NULL, 0, "%s/%s%s", cwd, gbl_data.filename, new_ending);

		if (path_len > MAX_EXPORT_PATH_LEN) {
			ODPH_ERR("Path too long, would overflow\n");
			return -1;
		}

		snprintf(gbl_data.path, MAX_EXPORT_PATH_LEN, "%s/%s%s", cwd, gbl_data.filename,
			 new_ending);
	}

	if (gbl_data.common_options.is_export) {
		gbl_data.file = fopen(gbl_data.path, "w");
		if (!gbl_data.file) {
			ODPH_ERR("Failed to open file %s: %s\n", gbl_data.path, strerror(errno));
			return -1;
		}
	}

	return 0;
}

ODP_WEAK_SYMBOL ODP_PRINTF_FORMAT(1, 2)
int test_common_write(const char *fmt, ...)
{
	va_list args, args_copy;
	int len, ret;

	va_start(args, fmt);
	va_copy(args_copy, args);

	len = vsnprintf(NULL, 0, fmt, args);

	ret = vfprintf(gbl_data.file, fmt, args_copy);

	va_end(args);
	va_end(args_copy);

	if (len != ret) {
		ODPH_ERR("Expected %i characters to be written, actually wrote: %i", len, ret);
		return -1;
	}

	return 0;
}

void test_common_write_term(void)
{
	if (gbl_data.file == NULL) {
		ODPH_ERR("Warning: there is no open file to be closed\n");
		return;
	}
	(void)fclose(gbl_data.file);
}
