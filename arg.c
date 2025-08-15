#include "arg.h"
#include <stdio.h>
#include <string.h>

static int set_dimensions(char *arg, struct program_vars *vars);
static int set_range(char *arg, struct program_vars *vars);
static int set_julia(char *arg, struct program_vars *vars);
static int set_single_var(char *arg, unsigned int *var);
static int print_help(void);
static int is_numeric(const char *string);

int handle_args(char *arg, struct program_vars *vars)
{
	if (*arg != '-' && vars->file_name != NULL) {
		fprintf(stderr, "Too many arguments\n");
		return 0;
	} else if (*arg != '-') {
		vars->file_name = arg;
		return 1;
	}
	char *option = arg;
	++arg;

	switch (*arg) {
	case 'd':
		if (set_dimensions(++arg, vars))
			break;
		goto INVALID_ARG;
	case 'r':
		if (set_range(++arg, vars))
			break;
		goto INVALID_ARG;
	case 'j':
		if (set_julia(++arg, vars))
			break;
		goto INVALID_ARG;
	case 'i':
		if (set_single_var(++arg, &(vars->max_iters)))
			break;
		goto INVALID_ARG;
	case 't':
		if (arg[1] == '\0') {
			vars->show_time = 1;
			break;
		}
		goto INVALID_ARG;
	case '-':
		if (strcmp(++arg, "help") == 0) {
			print_help();
			exit(0);
		}
		goto INVALID_ARG;
	INVALID_ARG:
	default:
		fprintf(stderr, "Invalid option '%s'\n", option);
		return 0;
	}

	return 1;
}

static int set_dimensions(char *arg, struct program_vars *vars)
{
	int len = 0;
	char **tokens = tokenize(arg, "x", &len);
	if (!tokens) {
		return 0;
	}

	if (len == 2 && is_numeric(tokens[0]) && is_numeric(tokens[1])) {
		vars->width = atoi(tokens[0]);
		vars->height = atoi(tokens[1]);
		if (vars->width == 0 || vars->height == 0)
			len = 0;
	} else {
		len = 0;
	}
	destroy_tokens(tokens);

	return len == 2;
}

static int set_range(char *arg, struct program_vars *vars)
{
	int len = 0;
	char **tokens = tokenize(arg, "x", &len);
	if (!tokens) {
		return 0;
	}

	if (len == 2) {
		char *end0;
		char *end1;
		vars->r_left = strtod(tokens[0], &end0);
		vars->r_right = strtod(tokens[1], &end1);
		if ((vars->r_left == 0 && end0== tokens[0])
				|| (vars->r_right == 0 && end1 == tokens[1]))
			len = 0;
	} else {
		len = 0;
	}
	destroy_tokens(tokens);

	return len == 2;
}

static int set_julia(char *arg, struct program_vars *vars)
{
	int len = 0;
	char **tokens = tokenize(arg, "x", &len);
	if (!tokens) {
		return 0;
	}

	vars->is_julia = 1;

	if (len == 2) {
		char *end0;
		char *end1;
		vars->c_r = strtod(tokens[0], &end0);
		vars->c_i = strtod(tokens[1], &end1);
		if ((vars->r_left == 0 && end0== tokens[0])
				|| (vars->r_right == 0 && end1 == tokens[1]))
			len = 0;
	} else {
		len = 0;
	}
	destroy_tokens(tokens);

	return len == 2;
}

static int set_single_var(char *arg, unsigned int *var)
{
	if (!is_numeric(arg))
		return 0;
	*var = atoi(arg);
	return 1;
}

static int print_help(void)
{
	FILE *f = fopen(".help", "r");
	if (!f) {
		fprintf(stderr, "README missing, you're on your own.\n");
		return 0;
	}

	size_t capacity = 0;
	char *line = NULL;
	
	while (getline(&line, &capacity, f) >= 0) {
		printf("%s", line);
		free(line);
		line = NULL;
	}
	if (line) {
		free(line);
	}

	fclose(f);
	return 1;
}

static int is_numeric(const char *string)
{
	if (string == NULL || *string == '\0')
		return 0;
	for (; *string; ++string) {
		if ('0' > *string || *string > '9')
			return 0;
	}

	return 1;
}
