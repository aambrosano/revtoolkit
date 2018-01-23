#ifndef ARCHIVES_GAMEBOY_GB_H
#define ARCHIVES_GAMEBOY_GB_H

#include "common/types.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace rev {
namespace archives {
namespace gameboy {

typedef uword_t addr_t;

static std::map<byte_t, std::string>
    licensee_map({{0x00, "none"},
                  {0x01, "nintendo"},
                  {0x08, "capcom"},
                  {0x13, "electronic arts"},
                  {0x18, "hudsonsoft"},
                  {0x19, "b:ai"},
                  {0x20, "kss"},
                  {0x22, "pow"},
                  {0x24, "pcm complete"},
                  {0x25, "san:x"},
                  {0x28, "kemco japan"},
                  {0x29, "seta"},
                  {0x30, "viacom"},
                  {0x31, "nintendo"},
                  {0x32, "bandia"},
                  {0x33, "ocean/acclaim"},
                  {0x34, "konami"},
                  {0x35, "hector"},
                  {0x37, "taito"},
                  {0x38, "hudson"},
                  {0x39, "banpresto"},
                  {0x41, "ubi soft"},
                  {0x42, "atlus"},
                  {0x44, "malibu"},
                  {0x46, "angel"},
                  {0x47, "pullet:proof"},
                  {0x49, "irem"},
                  {0x50, "absolute"},
                  {0x51, "acclaim"},
                  {0x52, "activision"},
                  {0x53, "american sammy"},
                  {0x54, "konami"},
                  {0x55, "hi tech entertainment"},
                  {0x56, "ljn"},
                  {0x57, "matchbox"},
                  {0x58, "mattel"},
                  {0x59, "milton bradley"},
                  {0x60, "titus"},
                  {0x61, "virgin"},
                  {0x64, "lucasarts"},
                  {0x67, "ocean"},
                  {0x69, "electronic arts"},
                  {0x70, "infogrames"},
                  {0x71, "interplay"},
                  {0x72, "broderbund"},
                  {0x73, "sculptured"},
                  {0x75, "sci"},
                  {0x78, "t*hq"},
                  {0x79, "accolade"},
                  {0x80, "misawa"},
                  {0x83, "lozc"},
                  {0x86, "tokuma shoten i*"},
                  {0x87, "tsukuda ori*"},
                  {0x91, "chun soft"},
                  {0x92, "video system"},
                  {0x93, "ocean/acclaim"},
                  {0x95, "varie"},
                  {0x96, "yonezawa/s'pal"},
                  {0x97, "kaneko"},
                  {0x99, "pack in soft"}

    });

static std::map<byte_t, udword_t> size_map({{0x00, 32768},
                                            {0x01, 65536},
                                            {0x02, 131072},
                                            {0x03, 262144},
                                            {0x04, 524288},
                                            {0x05, 1048576},
                                            {0x06, 2097152},
                                            {0x07, 4194304}});

typedef struct gb_header_t {
  dword_t entry_point;
  byte_t logo[0x30];
  byte_t title[0x0f];
  byte_t colour_compatibility;
  byte_t licensee[2];
  byte_t gb_sgb_function;
  byte_t cartridge_type;
  byte_t rom_size;
  byte_t ram_size;
  byte_t destination_code;
  byte_t old_licensee;
  byte_t mask_rom_version;
  byte_t compl_checksum;
  word_t checksum;

  void print();
  const static struct gb_header_t *fromFile(std::string filename);
} __attribute__((packed)) gb_header_t;

typedef char *gb_cartridge_t;
}
}
}

#endif
