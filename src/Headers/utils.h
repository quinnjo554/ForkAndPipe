#include <fstream>
#include <iostream>
#include <string>
class Utils {
public:
  static bool isNewLine(char ch);
  static int countLines(const std::string &fileName);
  static std::string getFileName(int fileIndex);
};
