#pragma once

#include <string>
#include <vector>

#include "fetalib/cli/fetalib_export.hpp"

namespace feta {

struct FETALIB_EXPORT Argument {
  std::string key;
  std::string alternate_key;
  std::string help_message;
  bool is_flag;
  bool is_optional;
  int word_count;
};

class FETALIB_EXPORT ArgumentParser {
public:
  ArgumentParser(int argc, char **argv) : argc {argc} {
    for (int i = 0; i < argc; i++) {
      this->argv.push_back(argv[i]);
    }
  }

private:
  int argc;
  std::vector<std::string> argv;
  std::vector<Argument> args;
};

}
