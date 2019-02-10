// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "types.h"

bool operator==(const map_point &a, const map_point &b) {
  return a.x == b.x && a.y == b.y;
}