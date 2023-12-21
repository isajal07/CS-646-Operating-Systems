#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

// Changes the current working directory to the specified path
// parameter 'path': The path to change the current working directory to
// returns: int 0 if successful, 1 otherwise
int changeDirectories(char* path) {
  if (chdir(path) != 0) {
    perror("Path Not Found!");
    return 1;
  }
  return 0;
}

// Parses the input string into separate arguments
// parameter 'input': The input string to parse
// parameter 'splitWords': The 2D char array to store the parsed arguments
// returns: int The number of arguments parsed from the input string
int parseInput(char* input, char** splitWords) {
  int wordInd = 0;
  splitWords[0] = strtok(input, " \n");
  while (splitWords[wordInd] != NULL) {
    splitWords[++wordInd] = strtok(NULL, " \n");
  }
  return wordInd;
}

// Executes the command provided by the user
// parameter 'splitWords': The array of command arguments
// returns: int 0 if successful, 1 otherwise
int executeCommand(char** splitWords) {
  pid_t pid;
  int status;

  if (strcmp(splitWords[0], "cd") == 0) {
    return changeDirectories(splitWords[1]);
  } else {
    pid = fork();
    if (pid < 0) {
      perror("fork failed!");
      return 1;
    } else if (pid == 0) {
      // Child process
      execvp(splitWords[0], splitWords);
      perror("Error: Failed to execute command");
      exit(1);
    } else {
      // Parent process
      waitpid(pid, &status, 0);
      if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
      } else {
        return 1;
      }
    }
  }
}

int main() {
  char input[100];
  int status;

  while (1) {
    printf("exampleShell\\8001635206$ ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      perror("Error: Failed to read input");
      break;
    }

    if (input[strlen(input) - 1] == '\n') {
      input[strlen(input) - 1] = '\0';
    }

    if (strcmp(input, "exit") == 0) {
      break;
    }

    char* splitWords[100];
    int numArgs = parseInput(input, splitWords);

    status = executeCommand(splitWords);
    if (status != 0) {
      fprintf(stderr, "Error: Failed to execute command\n");
    }
  }

  return 0;
}
