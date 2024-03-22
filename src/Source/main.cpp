#include "Process.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

bool isNewLine(char ch) { return ch == '\n' || ch == '\r'; }
int main() {
  int num_processes;
  std::cout << "Enter the number of child processes to create (1, 2, or 4): ";
  std::cin >> num_processes;

  int file_index;
  std::string fileName;
  std::cout << "Enter the file to process (1, 2, or 3): ";

  std::cin >> file_index;
  int total_lines;
  int lines_to_process;
  if (file_index == 1) {
    fileName = "file1.dat";
  } else if (file_index == 2) {
    fileName = "file2.dat";
  } else if (file_index == 3) {
    fileName = "file3.dat";
  }
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Unable to open file\n";
    return 1;
  }

  std::string line;
  int sum = 0;
  int lineCount = 0; // Counter for lines
  int fds_array[num_processes][2];

  while (std::getline(file, line)) {
    sum += std::stoi(line);
    ++lineCount; // Increment line count
  }

  lines_to_process = lineCount / num_processes;

  long long result = 0;
  std::cout << lines_to_process << "\n";
  //create pipes for communitcations
  for (int i = 0; i < num_processes; ++i) {
    if (pipe(fds_array[i]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}
for (int i = 0; i < num_processes; ++i) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        close(fds_array[i][0]); // Close unused read end
        Process child(i, fileName, lines_to_process, &fds_array[i][0]); // Create Process object
        child.openFile(fileName); // Open file
        int result = child.processFile(); // Process file
        child.closeFile(); // Close file

        // Write result to pipe
        write(fds_array[i][1], &result, sizeof(result));
        close(fds_array[i][1]); // Close write end of pipe

        return 0; // Exit child process
    } else { // Parent process
        close(fds_array[i][1]); // Close unused write end
    }
}

// Parent process reads and prints results
int total_result = 0;
for (int i = 0; i < num_processes; ++i) {
    int result;
    read(fds_array[i][0], &result, sizeof(result)); // Read result from pipe
    total_result += result;
    std::cout << "Result from child " << i << ": " << result << std::endl;
}
std::cout << "Total final result: " << total_result << std::endl;
  return 0;
}
