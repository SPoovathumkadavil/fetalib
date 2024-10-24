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
  BLUE=94,
  YELLOW=93,
  GREEN=92
};

static std::string colorize(std::string text, Color c) {
  return std::string("\033[").append(std::to_string(c)).append("m").append(text).append("\033[0m");
};

}  // namespace feta
