#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"
/**
* read_line - Read a line of input from stdin
* Return: Pointer to the input line read
*/
char *read_line(void)
{
char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
if (!buffer)
{
perror("malloc error");
exit(EXIT_FAILURE);
}
if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
{
free(buffer);
return NULL;
}
// Remove trailing newline
size_t len = strlen(buffer);
if (buffer[len - 1] == '\n')
buffer[len - 1] = '\0';
return buffer;
}
/**
* parse_line - Parse the input line into tokens
* @line: Input line to parse
* Return: Array of tokens (commands and arguments)
*/
char **parse_line(char *line)
{
int bufsize = BUFFER_SIZE, position = 0;
char **tokens = malloc(bufsize * sizeof(char *));
char *token;
if (!tokens)
{
perror("malloc error");
exit(EXIT_FAILURE);
}
token = strtok(line, TOKEN_DELIMITERS);
while (token != NULL)
{
tokens[position] = token;
position++;
if (position >= bufsize)
{
bufsize += BUFFER_SIZE;
tokens = realloc(tokens, bufsize * sizeof(char *));
if (!tokens)
{
perror("realloc error");
exit(EXIT_FAILURE);
}
}
token = strtok(NULL, TOKEN_DELIMITERS);
}
tokens[position] = NULL;
return tokens;
}
int main(void)
{
char *line;
char **args;
pid_t pid;
int status;
printf("#cisfun$ ");
while ((line = read_line()) != NULL)
{
if (strcmp(line, "exit") == 0)
{
free(line);
exit(EXIT_SUCCESS);
}
args = parse_line(line);
pid = fork();
if (pid == 0)
{
if (execvp(args[0], args) == -1)
{
perror("execvp error");
exit(EXIT_FAILURE);
}
}
else if (pid < 0)
{
perror("fork error");
}
else
{
do
{
waitpid(pid, &status, WUNTRACED);
} while (!WIFEXITED(status) && !WIFSIGNALED(status));
}
printf("#cisfun$ ");
free(line);
free(args);
}
return 0;
}
