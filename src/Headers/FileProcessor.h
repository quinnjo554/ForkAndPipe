#include <fstream>
#include <iostream>

class FileProcessor {
  public:
    virtual ~FileProcessor(){}
    virtual bool openFile(const std::string& fileName) = 0;
    virtual long long processFile() = 0;
    virtual void closeFile() = 0;
    
};
