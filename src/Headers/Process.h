#include "FileProcessor.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

class Process : public FileProcessor {
public:

  Process(int id, const std::string &filename, int numLines, int *fds);
  virtual ~Process();
  bool openFile(const std::string &filename) override;
  long long processFile() override;
  void closeFile() override;

private:
  int id_;
  int* fds_;
  std::string filename_;
  int numLines_;
  std::fstream file_;
};
