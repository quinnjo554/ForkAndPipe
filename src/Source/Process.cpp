#include "Process.h"
#include <unistd.h>

Process::Process(int id, const std::string &filename, int numLines)
    : id_(id), filename_(filename), numLines_(numLines) {
  pipe(fds); // init fds
}

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

  int write_result = write(fds[1], &sum, sizeof(sum));
  if (write_result == -1) {
    return -1;
  }
  return sum;
}

void Process::closeFile() { file_.close(); }
