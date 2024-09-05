#pragma once

#include <unistd.h>
#include <string>

#include "fetalib/cli/fetalib_export.hpp"

namespace feta {

enum class FETALIB_EXPORT color : uint32_t {
  beige=0xC7AE8C,
  red=0x689F94,
  purple=0xC8748A,
  cyan=0x6E9C71,
  blue=0x97A65F,
  green=0x97A65F,
  yellow=0xCE9554
};

}

