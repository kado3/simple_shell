#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
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
int main(void)
{
char *line;
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
pid = fork();
if (pid == 0)
{
if (execlp(line, line, NULL) == -1)
{
fprintf(stderr, "%s: command not found\n", line);
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
}
return 0;
}
