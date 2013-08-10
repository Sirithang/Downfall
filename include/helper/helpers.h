#pragma once

#include <stdint.h>

uint32_t reverseBytes(uint32_t value)
{
  return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
         (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}