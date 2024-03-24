#include "utils.h"
#include <string>

bool Utils::isNewLine(char ch) { return ch == '\n' || ch == '\r'; }

int Utils::countLines(const std::string &fileName) {
  std::ifstream file(fileName);
  if (!file) {
    std::cerr << "Unable to open file\n";
    return -1;
  }

  std::string line;
  int lineCount = 0; // Counter for lines

  while (std::getline(file, line)) {
    ++lineCount; // Increment line count
  }

  return lineCount;
}

std::string Utils::getFileName(int fileIndex) {
  if (fileIndex == 1) {
    return "file1.dat";
  } else if (fileIndex == 2) {
    return "file2.dat";
  } else if (fileIndex == 3) {
    return "file3.dat";
  } else {
    return "";
  }
}
