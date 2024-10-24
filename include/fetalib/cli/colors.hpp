#pragma once

#include <string>

#include "fetalib/cli/fetalib_export.hpp"

namespace feta
{
enum Color {
  RED=31,
  PURPLE=95,
  CYAN=96,
  DARK_CYAN=36,
  BLUE=92,
  YELLOW=93
};

static std::string colorize(std::string text, Color c) {
  return "\033[" + c + "m" + text + "\033[0m";
};

}  // namespace feta
