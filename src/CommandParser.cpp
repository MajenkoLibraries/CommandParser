#include <CommandParser.h>

CommandParser::CommandParser() {
    commands = NULL;
    _caseSensitive = true;
}

void CommandParser::setCaseInsensitive() {
    _caseSensitive = false;
}

void CommandParser::setCaseSensitive() {
    _caseSensitive = true;
}

boolean CommandParser::isCaseSensitive() {
    return _caseSensitive;
}

void CommandParser::addCommand(const char *command, int (*function)(int, char **)) {
    CLICommand *scan;
    CLICommand *newCommand;

    newCommand = (CLICommand *)malloc(sizeof(CLICommand));
    newCommand->command = strdup(command);
    newCommand->flags = 0;
    newCommand->function = function;
    newCommand->next = NULL;

    if (commands == NULL) {
        commands = newCommand;
        return;
    }
    for (scan = commands; scan->next; scan = scan->next);
    scan->next = newCommand;
}

void CommandParser::addPrefix(const char *command, int (*function)(int, char **)) {
    CLICommand *scan;
    CLICommand *newCommand;

    newCommand = (CLICommand *)malloc(sizeof(CLICommand));
    newCommand->command = strdup(command);
    newCommand->flags = CLI_IS_PREFIX;
    newCommand->function = function;
    newCommand->next = NULL;

    if (commands == NULL) {
        commands = newCommand;
        return;
    }
    for (scan = commands; scan->next; scan = scan->next);
    scan->next = newCommand;
}

static inline char *getWord(char *buf) {
    static char *ptr = NULL;
    char *start, *scan;
    char term = ' ';

    if (buf != NULL) {
        ptr = buf;
    }

    while ((*ptr == ' ' || *ptr == '\t') && *ptr != '\0') {
        ptr++;
    }
    if (*ptr == '\0') {
        return NULL;
    }

    if (*ptr == '"' || *ptr == '\'') {
        term = *ptr;
        ptr++;
    }
    start = ptr;

    while (*ptr != '\0') {
        if (*ptr == '\\') {
            for (scan = ptr; *scan != '\0'; scan++) {
                *scan = *(scan+1);
            }
            ptr++;
            continue;
        }
        if (*ptr == term || (term == ' ' && *ptr == '\t')) {
            *ptr = '\0';
            ptr++;
            return start;
        }
        ptr++;
    }
    if (ptr == start) {
        return NULL;
    }
    return start;
}

int CommandParser::process(char *input) {
    CLICommand *scan;
	char *argv[20];
	int argc;
	char *w;

	argc = 0;
	w = getWord(input);
	while ((argc < 20) && (w != NULL)) {
		argv[argc++] = w;
		w = getWord(NULL);
	}
    if (isCaseSensitive()) {
        for (scan = commands; scan; scan = scan->next) {
            if ((scan->flags & CLI_IS_PREFIX) == 0) {
                if (strcmp(scan->command, argv[0]) == 0) {
                    return scan->function(argc, argv);
                }
            }
        }
        for (scan = commands; scan; scan = scan->next) {
            if ((scan->flags & CLI_IS_PREFIX) != 0) {
                if (strncmp(scan->command, argv[0], strlen(scan->command)) == 0) {
                    return scan->function(argc, argv);
                }
            }
        }
    } else {
        for (scan = commands; scan; scan = scan->next) {
            if ((scan->flags & CLI_IS_PREFIX) == 0) {
                if (strcasecmp(scan->command, argv[0]) == 0) {
                    return scan->function(argc, argv);
                }
            }
        }
        for (scan = commands; scan; scan = scan->next) {
            if ((scan->flags & CLI_IS_PREFIX) != 0) {
                if (strncasecmp(scan->command, argv[0], strlen(scan->command)) == 0) {
                    return scan->function(argc, argv);
                }
            }
        }
    }
	return -1;
}
