#include "Process.h"
#include "utils.h"
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
bool isNewLine(char ch) { return ch == '\n' || ch == '\r'; }

int main() {

  int total_result = 0;
  int num_processes;
  std::cout << "Enter the number of child processes to create (1, 2, or 4): ";
  std::cin >> num_processes;

  int file_index;
  std::cout << "Enter the file to process (1, 2, or 3): ";

  int fds_array[num_processes][2];

  std::cin >> file_index;
  std::string fileName = Utils::getFileName(file_index);
  int lineCount = Utils::countLines(fileName);
  int lines_to_process = lineCount / num_processes;

  // create pipes for communitcations
  for (int i = 0; i < num_processes; ++i) {
    if (pipe(fds_array[i]) == -1) {
      perror("pipe");
      exit(EXIT_FAILURE);
    }
  }

  auto start = std::chrono::high_resolution_clock::now(); // Start time

  for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();

    if (pid < 0) {
      std::cerr << "Fork failed." << std::endl;
      exit(EXIT_FAILURE);
    } else if (pid == 0) {    // Child process
      close(fds_array[i][0]); // Close unused read end

      Process child(i, fileName, lines_to_process,
                    &fds_array[i][0]); // Create Process object

      child.openFile(fileName);
      int result = child.processFile();
      child.closeFile();

      // Write result to pipe
      write(fds_array[i][1], &result, sizeof(result));
      close(fds_array[i][1]);
      return 0;
    } else {
      close(fds_array[i][1]); // Close unused write end
    }
  }
  // wait for children to finish
  wait(NULL);
  // End timer
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << num_processes << " processes took " << duration.count()
            << " milliseconds. \n";

  // Parent process reads and prints results
  for (int i = 0; i < num_processes; ++i) {
    int result;
    read(fds_array[i][0], &result, sizeof(result)); // Read result from pipe
    total_result += result;
  }
  std::cout << "Total final result: " << total_result << std::endl;
  return 0;
}
