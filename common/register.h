#ifndef COMMON_REGISTER_H
#define COMMON_REGISTER_H

#include "common/types.h"

typedef struct le_word_t {
  byte_t high;
  byte_t low;
} __attribute__((packed)) le_word_t;

typedef struct be_word_t {
  byte_t low;
  byte_t high;
} __attribute__((packed)) be_word_t;

typedef struct register_t {
  char name[10];
  union {
    byte_t byte;
    be_word_t be_word;
    le_word_t le_word;
    dword_t dword;
    qword_t qword;
  } value;
} __attribute__((packed)) register_t;

#endif // COMMON_REGISTER_H
