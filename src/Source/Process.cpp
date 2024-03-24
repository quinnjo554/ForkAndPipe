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
  if (!file_.is_open()) {
    return -1;
  }

  long long sum = 0;
  int lineCount = 0;
  std::string line;

  // skip lines processed by previous child processes
  for (int i = 0; i < id_ * numLines_; ++i) {
    if (!std::getline(file_, line)) {
      perror("getline");
      return -1;
    }
  }

  // process assigned lines
  while (lineCount < numLines_ && std::getline(file_, line)) {
    int number;
    std::stringstream ss(line);
    ss >> number;
    sum += number;
    lineCount++;
  }

  // write the sum to parent thorugh pipe
  int write_result = write(fds_[1], &sum, sizeof(sum));

  if (write_result == -1) {
    perror("write");
    return -1;
  }

  return sum;
}

void Process::closeFile() {
  file_.close();
  close(fds_[0]);
  close(fds_[1]);
}
