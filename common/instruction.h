#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common/types.h"
#include <string>

typedef struct instr_view_t {
  byte_t *instr;
  byte_t sz;
  std::string str;

  instr_view_t(byte_t *i, byte_t s, std::string t) : instr(i), sz(s), str(t) {}
} instr_view_t;

#endif // INSTRUCTION_H
