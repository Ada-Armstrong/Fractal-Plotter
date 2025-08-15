#include "tokenize.h"

char **tokenize(char *input, const char *delim, int *len)
{
       	int max_tokens = 10;
	char **tokens = malloc(max_tokens * sizeof(char *));
	if (!tokens)
		return NULL;

	int index = 0;
	char *token = strtok(input, delim);

	while(token != NULL) {
		if (index >= max_tokens - 1) {
			max_tokens *= 2;
			tokens = realloc(tokens, max_tokens * sizeof(char *));
			if (!tokens)
				return NULL;
		}
		char *cpy = malloc((strlen(token) + 1) * sizeof(char));
		if (!cpy) {
			destroy_tokens(tokens);
			return NULL;
		}
		tokens[index] = strcpy(cpy, token);
		token = strtok(NULL, delim);
		++index;
	}
	// resize to remove unused memory
	tokens = realloc(tokens, (index + 1) * sizeof(char *));
	if (!tokens)
		return NULL;
	tokens[index] = NULL;
	*len = index;

	return tokens;
}

void destroy_tokens(char **tokens)
{
	if (!tokens)
		return;
	for (int i = 0; tokens[i]; ++i) {
		free(tokens[i]);
	}
	free(tokens);
}
