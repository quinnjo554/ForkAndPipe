#include "Process.h"
#include <iostream>
#include <unistd.h>

int main() {
  int num_processes;
  std::cout << "Enter the number of child processes to create (1, 2, or 4): ";
  std::cin >> num_processes;

  int file_index;
  std::cout << "Enter the index of the file to process (1, 2, or 3): ";
  std::cin >> file_index;

  for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();
    if (pid < 0) {
      std::cerr << "Fork failed." << std::endl;
      return 1;
    } else if (pid == 0) {
      Process child(i, "data1.txt",
                    1); // todo, calc line nums and get files
      child.openFile();
      long long result = child.processFile();
      child.closeFile();
      // result to parent
      write(pipe_fd[i], &result, sizeof(result));
      return 0; // Exit child process
    }
  }

  for (int i = 0; i < num_processes; ++i) {
    read(pipe_fd[i], &result, sizeof(result));
    // print result
  }

  // todo calculate total

  return 0;
}
