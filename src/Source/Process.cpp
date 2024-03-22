#include "Process.h"
#include <sstream>

Process::Process(int id, const std::string &filename, int numLines, int *fds)
    : id_(id), filename_(filename), numLines_(numLines), fds_(fds) {}

Process::~Process() { closeFile(); }

bool Process::openFile(const std::string &filename) {
  file_.open(filename_, std::ios::in);
  return file_.is_open();
}

long long Process::processFile() {
  // pipe the result to the parent process
  if (!file_.is_open()) {
    return -1;
  }

  long long sum = 0;
  int lineCount = 0;
  std::string line;

  file_.seekg(id_ * numLines_ * sizeof(int), std::ios::beg);

  while (lineCount < numLines_ && std::getline(file_, line)) {
    int number;
    std::stringstream ss(line);
    ss >> number;
    sum += number;
    lineCount++;
  }

  // Write the sum to the parent process through the pipe
  int write_result = write(fds_[1], &sum, sizeof(sum));
  if (write_result == -1) {
    perror("write");
    return -1;
  }
  return sum; // this is the correct number;
}

void Process::closeFile() {
  file_.close();
  close(fds_[0]); // Close the reading end of the pipe
  close(fds_[1]); // Close the writing end of the pipe }
  }
