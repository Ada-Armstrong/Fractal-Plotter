#include "arg.h"

static int set_dimensions(char *arg, struct program_vars *vars);
static int set_single_var(char *arg, unsigned int *var);
static int print_help();
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
	++arg;

	switch (*arg) {
	case 'd':
		if (set_dimensions(++arg, vars))
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
	INVALID_ARG:
	default:
		fprintf(stderr, "Invalid option '%s'\n", arg);
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

static int print_help()
{
	FILE *f = fopen("README", "r");
	if (!f) {
		fprintf(stderr, "README missing, you're on your own.\n");
		return 0;
	}

	size_t capacity = 0;
	char *line;
	
	while (1) {
		line = NULL;
		if (getline(&line, &capacity, f) < 0) {
			free(line);
			break;
		}
		printf("%s", line);
		free(line);
	}

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
