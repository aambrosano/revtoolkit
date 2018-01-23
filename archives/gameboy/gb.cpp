#include "archives/gameboy/gb.h"
#include "common/instruction.h"
#include <algorithm>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

namespace rev {
namespace archives {
namespace gameboy {

void logo_print(gb_header_t &header) {
  std::cout << "Logo: " << std::endl;
  for (int j = 0; j < 2; j++) {
    for (int k = 0; k < 2; k++) {
      for (int i = 0x18 * j + k; i < 0x18 * (j + 1); i += 2) {
        std::cout << ((header.logo[i] & 0x80) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x40) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x20) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x10) ? "\u2588" : " ");
      }
      std::cout << std::endl;
      for (int i = 0x18 * j + k; i < 0x18 * (j + 1); i += 2) {
        std::cout << ((header.logo[i] & 0x08) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x04) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x02) ? "\u2588" : " ");
        std::cout << ((header.logo[i] & 0x01) ? "\u2588" : " ");
      }
      std::cout << std::endl;
    }
  }
}

void title_print(gb_header_t &header) {
  std::cout << "Title: ";
  for (int i = 0; i < 15; i++)
    std::cout << ((char *)(&header.title[0]))[i];
  std::cout << std::endl;
}

void print_hex(std::string desc, int size, int x, bool newline = true) {
  std::cout << desc << ": 0x" << std::setw(size) << std::setfill('0')
            << std::hex << x;
  if (newline)
    std::cout << std::endl;
}

void gb_header_t::print() {
  print_hex("entry point:", 4, this->entry_point + 0x100);
  logo_print(*this);
  title_print(*this);
  print_hex("colour_compat", 1, this->colour_compatibility);
  std::cout << "licensee: " << this->licensee[0] << this->licensee[1]
            << std::endl;
  print_hex("gb_sgb_function", 1, this->gb_sgb_function);
  print_hex("cartridge_type", 1, this->cartridge_type);
  std::cout << "rom_size: " << std::dec << size_map[this->rom_size]
            << std::endl;
  std::cout << "ram_size: " << std::dec << size_map[this->ram_size]
            << std::endl;
  print_hex("destination_code", 1, this->destination_code);
  print_hex("old_licensee", 1, this->old_licensee);
  print_hex("mask_rom_version", 1, this->mask_rom_version);
  print_hex("compl_checksum", 1, this->compl_checksum);
  print_hex("checksum", 2, this->checksum);
}

void add_branch(std::vector<addr_t> *branches, addr_t branch) {
  if (std::find(branches->begin(), branches->end(), branch) == branches->end())
    branches->push_back(branch);
}

void add_function(std::vector<addr_t> *functions, addr_t function) {
  if (std::find(functions->begin(), functions->end(), function) ==
      functions->end())
    functions->push_back(function);
}

void decode(gb_cartridge_t c, addr_t off) {
  std::vector<addr_t> branches = {};
  std::vector<std::map<char *, char *>> xrefs;
  std::vector<instr_view_t> instructions;
  std::vector<addr_t> funcs = {};

  addr_t i = off;
  int cnt = 0;
  int branch_i = 0;
  int func_i = 0;
  // while (cnt != 1000) {
  do {
    bool end = false;
    switch ((ubyte_t)c[i]) {
    case 0x00:
      instructions.push_back({&(c[i]), 1, "nop"});
      break;
    case 0x01:
      instructions.push_back({&(c[i]), 3, "ld bc, d16"});
      i += 2;
      break;
    case 0x02:
      instructions.push_back({&(c[i]), 1, "ld (bc), a"});
      break;
    case 0x03:
      instructions.push_back({&(c[i]), 1, "inc bc"});
      break;
    case 0x04:
      instructions.push_back({&(c[i]), 1, "inc b"});
      break;
    case 0x05:
      instructions.push_back({&(c[i]), 1, "dec b"});
      break;
    case 0x06:
      instructions.push_back({&(c[i]), 2, "ld b, d8"});
      i++;
      break;
    case 0x07:
      instructions.push_back({&(c[i]), 1, "rlca"});
      break;
    case 0x08:
      instructions.push_back({&(c[i]), 3, "ld (a16), sp"});
      i += 2;
      break;
    case 0x09:
      instructions.push_back({&(c[i]), 1, "add hl, bc"});
      break;
    case 0x0A:
      instructions.push_back({&(c[i]), 1, "ld a, (bc)"});
      break;
    case 0x0B:
      instructions.push_back({&(c[i]), 1, "dec bc"});
      break;
    case 0x0C:
      instructions.push_back({&(c[i]), 1, "inc c"});
      break;
    case 0x0D:
      instructions.push_back({&(c[i]), 1, "dec c"});
      break;
    case 0x0E:
      instructions.push_back({&(c[i]), 2, "ld c, d8"});
      i++;
      break;
    case 0x0F:
      instructions.push_back({&(c[i]), 1, "rrca"});
      break;

    case 0x10:
      instructions.push_back({&(c[i]), 1, "stop 0"});
      break;
    case 0x11:
      instructions.push_back({&(c[i]), 3, "ld de, b16"});
      i += 2;
      break;
    case 0x12:
      instructions.push_back({&(c[i]), 1, "ld (de), a"});
      break;
    case 0x13:
      instructions.push_back({&(c[i]), 1, "inc de"});
      break;
    case 0x14:
      instructions.push_back({&(c[i]), 1, "inc d"});
      break;
    case 0x15:
      instructions.push_back({&(c[i]), 1, "dec d"});
      break;
    case 0x16:
      instructions.push_back({&(c[i]), 2, "ld d, d8"});
      i++;
      break;
    case 0x17:
      instructions.push_back({&(c[i]), 1, "rla"});
      break;
    case 0x18:
      instructions.push_back({&(c[i]), 2, "jr r8"});
      i++;
      add_branch(&branches, i + c[i]);
      break;
    case 0x19:
      instructions.push_back({&(c[i]), 1, "add hl, de"});
      break;
    case 0x1A:
      instructions.push_back({&(c[i]), 1, "ld a, (de)"});
      break;
    case 0x1B:
      instructions.push_back({&(c[i]), 1, "dec de"});
      break;
    case 0x1C:
      instructions.push_back({&(c[i]), 1, "inc e"});
      break;
    case 0x1D:
      instructions.push_back({&(c[i]), 1, "dec e"});
      break;
    case 0x1E:
      instructions.push_back({&(c[i]), 2, "ld e, d8"});
      i++;
      break;
    case 0x1F:
      instructions.push_back({&(c[i]), 1, "rra"});
      break;
    case 0x20:
      instructions.push_back({&(c[i]), 2, "jr nz, r8"});
      i++;
      add_branch(&branches, i + c[i]);
      break;
    case 0x21:
      instructions.push_back({&(c[i]), 3, "ld hl, d16"});
      i += 2;
      break;
    case 0x22:
      instructions.push_back({&(c[i]), 1, "ld (hl+), a"});
      break;
    case 0x23:
      instructions.push_back({&(c[i]), 1, "inc hl"});
      break;
    case 0x24:
      instructions.push_back({&(c[i]), 1, "inc "});
      break;
    case 0x25:
      instructions.push_back({&(c[i]), 1, "dec h"});
      break;
    case 0x26:
      instructions.push_back({&(c[i]), 2, "ld h, d8"});
      i++;
      break;
    case 0x27:
      instructions.push_back({&(c[i]), 1, "daa"});
      break;
    case 0x28:
      instructions.push_back({&(c[i]), 2, "jr z, r8"});
      i++;
      add_branch(&branches, i + c[i]);
      break;
    case 0x29:
      instructions.push_back({&(c[i]), 1, "add hl, hl"});
      break;
    case 0x2A:
      instructions.push_back({&(c[i]), 1, "ld a, (hl+)"});
      break;
    case 0x2B:
      instructions.push_back({&(c[i]), 1, "dec hl"});
      break;
    case 0x2C:
      instructions.push_back({&(c[i]), 1, "inc l"});
      break;
    case 0x2D:
      instructions.push_back({&(c[i]), 1, "dec l"});
      break;
    case 0x2E:
      instructions.push_back({&(c[i]), 2, "ld l, d8"});
      i++;
      break;
    case 0x2F:
      instructions.push_back({&(c[i]), 1, "cpl"});
      break;
    case 0x30:
      instructions.push_back({&(c[i]), 2, "jr nc, d8"});
      i++;
      add_branch(&branches, c[i]);

      break;
    case 0x31:
      instructions.push_back({&(c[i]), 3, "ld sp, d16"});
      i += 2;
      break;
    case 0x32:
      instructions.push_back({&(c[i]), 1, "ld (hl-), a"});
      break;
    case 0x33:
      instructions.push_back({&(c[i]), 1, "inc sp"});
      break;
    case 0x34:
      instructions.push_back({&(c[i]), 1, "inc (hl)"});
      break;
    case 0x35:
      instructions.push_back({&(c[i]), 1, "dec (hl)"});
      break;
    case 0x36:
      instructions.push_back({&(c[i]), 2, "ld (hl), d8"});
      i++;
      break;
    case 0x37:
      instructions.push_back({&(c[i]), 1, "scf"});
      break;
    case 0x38:
      instructions.push_back({&(c[i]), 2, "jr c, d8"});
      i++;
      add_branch(&branches, i + c[i]);
      break;
    case 0x39:
      instructions.push_back({&(c[i]), 1, "add hl, sp"});
      break;
    case 0x3A:
      instructions.push_back({&(c[i]), 1, "ld a, (hl-)"});
      break;
    case 0x3B:
      instructions.push_back({&(c[i]), 1, "dec sp"});
      break;
    case 0x3C:
      instructions.push_back({&(c[i]), 1, "inc a"});
      break;
    case 0x3D:
      instructions.push_back({&(c[i]), 1, "dec a"});
      break;
    case 0x3E:
      instructions.push_back({&(c[i]), 2, "ld a, d8"});
      i++;
      break;
    case 0x3F:
      instructions.push_back({&(c[i]), 1, "ccf"});
      break;

    case 0x40:
      instructions.push_back({&(c[i]), 1, "ld b, b"});
      break;
    case 0x41:
      instructions.push_back({&(c[i]), 1, "ld b, c"});
      break;
    case 0x42:
      instructions.push_back({&(c[i]), 1, "ld b, d"});
      break;
    case 0x43:
      instructions.push_back({&(c[i]), 1, "ld b, e"});
      break;
    case 0x44:
      instructions.push_back({&(c[i]), 1, "ld b, h"});
      break;
    case 0x45:
      instructions.push_back({&(c[i]), 1, "ld b, l"});
      break;
    case 0x46:
      instructions.push_back({&(c[i]), 1, "ld b, (hl)"});
      break;
    case 0x47:
      instructions.push_back({&(c[i]), 1, "ld b, a"});
      break;
    case 0x48:
      instructions.push_back({&(c[i]), 1, "ld c, b"});
      break;
    case 0x49:
      instructions.push_back({&(c[i]), 1, "ld c, c"});
      break;
    case 0x4A:
      instructions.push_back({&(c[i]), 1, "ld c, d"});
      break;
    case 0x4B:
      instructions.push_back({&(c[i]), 1, "ld c, e"});
      break;
    case 0x4C:
      instructions.push_back({&(c[i]), 1, "ld c, h"});
      break;
    case 0x4D:
      instructions.push_back({&(c[i]), 1, "ld c, l"});
      break;
    case 0x4E:
      instructions.push_back({&(c[i]), 1, "ld c, (hl)"});
      break;
    case 0x4F:
      instructions.push_back({&(c[i]), 1, "ld c, a"});
      break;

    case 0x50:
      instructions.push_back({&(c[i]), 1, "ld d, b"});
      break;
    case 0x51:
      instructions.push_back({&(c[i]), 1, "ld d, c"});
      break;
    case 0x52:
      instructions.push_back({&(c[i]), 1, "ld d, d"});
      break;
    case 0x53:
      instructions.push_back({&(c[i]), 1, "ld d, e"});
      break;
    case 0x54:
      instructions.push_back({&(c[i]), 1, "ld d, h"});
      break;
    case 0x55:
      instructions.push_back({&(c[i]), 1, "ld d, l"});
      break;
    case 0x56:
      instructions.push_back({&(c[i]), 1, "ld d, (hl)"});
      break;
    case 0x57:
      instructions.push_back({&(c[i]), 1, "ld d, a"});
      break;
    case 0x58:
      instructions.push_back({&(c[i]), 1, "ld e, b"});
      break;
    case 0x59:
      instructions.push_back({&(c[i]), 1, "ld e, c"});
      break;
    case 0x5A:
      instructions.push_back({&(c[i]), 1, "ld e, d"});
      break;
    case 0x5B:
      instructions.push_back({&(c[i]), 1, "ld e, e"});
      break;
    case 0x5C:
      instructions.push_back({&(c[i]), 1, "ld e, h"});
      break;
    case 0x5D:
      instructions.push_back({&(c[i]), 1, "ld e, l"});
      break;
    case 0x5E:
      instructions.push_back({&(c[i]), 1, "ld e, (hl)"});
      break;
    case 0x5F:
      instructions.push_back({&(c[i]), 1, "ld e, a"});
      break;

    case 0x60:
      instructions.push_back({&(c[i]), 1, "ld h, b"});
      break;
    case 0x61:
      instructions.push_back({&(c[i]), 1, "ld h, c"});
      break;
    case 0x62:
      instructions.push_back({&(c[i]), 1, "ld h, d"});
      break;
    case 0x63:
      instructions.push_back({&(c[i]), 1, "ld h, e"});
      break;
    case 0x64:
      instructions.push_back({&(c[i]), 1, "ld h, h"});
      break;
    case 0x65:
      instructions.push_back({&(c[i]), 1, "ld h, l"});
      break;
    case 0x66:
      instructions.push_back({&(c[i]), 1, "ld h, (hl)"});
      break;
    case 0x67:
      instructions.push_back({&(c[i]), 1, "ld h, a"});
      break;
    case 0x68:
      instructions.push_back({&(c[i]), 1, "ld l, b"});
      break;
    case 0x69:
      instructions.push_back({&(c[i]), 1, "ld l, c"});
      break;
    case 0x6A:
      instructions.push_back({&(c[i]), 1, "ld l, d"});
      break;
    case 0x6B:
      instructions.push_back({&(c[i]), 1, "ld l, e"});
      break;
    case 0x6C:
      instructions.push_back({&(c[i]), 1, "ld l, h"});
      break;
    case 0x6D:
      instructions.push_back({&(c[i]), 1, "ld l, l"});
      break;
    case 0x6E:
      instructions.push_back({&(c[i]), 1, "ld l, (hl)"});
      break;
    case 0x6F:
      instructions.push_back({&(c[i]), 1, "ld l, a"});
      break;

    case 0x70:
      instructions.push_back({&(c[i]), 1, "ld (hl), b"});
      break;
    case 0x71:
      instructions.push_back({&(c[i]), 1, "ld (hl), c"});
      break;
    case 0x72:
      instructions.push_back({&(c[i]), 1, "ld (hl), d"});
      break;
    case 0x73:
      instructions.push_back({&(c[i]), 1, "ld (hl), e"});
      break;
    case 0x74:
      instructions.push_back({&(c[i]), 1, "ld (hl), h"});
      break;
    case 0x75:
      instructions.push_back({&(c[i]), 1, "ld (hl), l"});
      break;
    case 0x76:
      instructions.push_back({&(c[i]), 1, "halt"});
      end = true;
      break;
    case 0x77:
      instructions.push_back({&(c[i]), 1, "ld (hl), a"});
      break;
    case 0x78:
      instructions.push_back({&(c[i]), 1, "ld a, b"});
      break;
    case 0x79:
      instructions.push_back({&(c[i]), 1, "ld a, c"});
      break;
    case 0x7A:
      instructions.push_back({&(c[i]), 1, "ld a, d"});
      break;
    case 0x7B:
      instructions.push_back({&(c[i]), 1, "ld a, e"});
      break;
    case 0x7C:
      instructions.push_back({&(c[i]), 1, "ld a, h"});
      break;
    case 0x7D:
      instructions.push_back({&(c[i]), 1, "ld a, l"});
      break;
    case 0x7E:
      instructions.push_back({&(c[i]), 1, "ld a, (hl)"});
      break;
    case 0x7F:
      instructions.push_back({&(c[i]), 1, "ld a, a"});
      break;

    case 0x80:
      instructions.push_back({&(c[i]), 1, "add a, b"});
      break;
    case 0x81:
      instructions.push_back({&(c[i]), 1, "add a, c"});
      break;
    case 0x82:
      instructions.push_back({&(c[i]), 1, "add a, d"});
      break;
    case 0x83:
      instructions.push_back({&(c[i]), 1, "add a, e"});
      break;
    case 0x84:
      instructions.push_back({&(c[i]), 1, "add a, h"});
      break;
    case 0x85:
      instructions.push_back({&(c[i]), 1, "add a, l"});
      break;
    case 0x86:
      instructions.push_back({&(c[i]), 1, "add a, (hl)"});
      break;
    case 0x87:
      instructions.push_back({&(c[i]), 1, "adc a, a"});
      break;
    case 0x88:
      instructions.push_back({&(c[i]), 1, "adc a, b"});
      break;
    case 0x89:
      instructions.push_back({&(c[i]), 1, "adc a, c"});
      break;
    case 0x8A:
      instructions.push_back({&(c[i]), 1, "adc a, d"});
      break;
    case 0x8B:
      instructions.push_back({&(c[i]), 1, "adc a, e"});
      break;
    case 0x8C:
      instructions.push_back({&(c[i]), 1, "adc a, h"});
      break;
    case 0x8D:
      instructions.push_back({&(c[i]), 1, "adc a, l"});
      break;
    case 0x8E:
      instructions.push_back({&(c[i]), 1, "adc a, (hl)"});
      break;
    case 0x8F:
      instructions.push_back({&(c[i]), 1, "adc a, a"});
      break;

    case 0x90:
      instructions.push_back({&(c[i]), 1, "sub b"});
      break;
    case 0x91:
      instructions.push_back({&(c[i]), 1, "sub c"});
      break;
    case 0x92:
      instructions.push_back({&(c[i]), 1, "sub d"});
      break;
    case 0x93:
      instructions.push_back({&(c[i]), 1, "sub e"});
      break;
    case 0x94:
      instructions.push_back({&(c[i]), 1, "sub h"});
      break;
    case 0x95:
      instructions.push_back({&(c[i]), 1, "sub l"});
      break;
    case 0x96:
      instructions.push_back({&(c[i]), 1, "sub (hl)"});
      break;
    case 0x97:
      instructions.push_back({&(c[i]), 1, "sub a"});
      break;
    case 0x98:
      instructions.push_back({&(c[i]), 1, "sbc b"});
      break;
    case 0x99:
      instructions.push_back({&(c[i]), 1, "sbc c"});
      break;
    case 0x9A:
      instructions.push_back({&(c[i]), 1, "sbc d"});
      break;
    case 0x9B:
      instructions.push_back({&(c[i]), 1, "sbc e"});
      break;
    case 0x9C:
      instructions.push_back({&(c[i]), 1, "sbc h"});
      break;
    case 0x9D:
      instructions.push_back({&(c[i]), 1, "sbc l"});
      break;
    case 0x9E:
      instructions.push_back({&(c[i]), 1, "sbc (hl)"});
      break;
    case 0x9F:
      instructions.push_back({&(c[i]), 1, "sbc a"});
      break;

    case 0xA0:
      instructions.push_back({&(c[i]), 1, "and b"});
      break;
    case 0xA1:
      instructions.push_back({&(c[i]), 1, "and c"});
      break;
    case 0xA2:
      instructions.push_back({&(c[i]), 1, "and d"});
      break;
    case 0xA3:
      instructions.push_back({&(c[i]), 1, "and e"});
      break;
    case 0xA4:
      instructions.push_back({&(c[i]), 1, "and h"});
      break;
    case 0xA5:
      instructions.push_back({&(c[i]), 1, "and l"});
      break;
    case 0xA6:
      instructions.push_back({&(c[i]), 1, "and (hl)"});
      break;
    case 0xA7:
      instructions.push_back({&(c[i]), 1, "and a"});
      break;
    case 0xA8:
      instructions.push_back({&(c[i]), 1, "xor b"});
      break;
    case 0xA9:
      instructions.push_back({&(c[i]), 1, "xor c"});
      break;
    case 0xAA:
      instructions.push_back({&(c[i]), 1, "xor d"});
      break;
    case 0xAB:
      instructions.push_back({&(c[i]), 1, "xor e"});
      break;
    case 0xAC:
      instructions.push_back({&(c[i]), 1, "xor h"});
      break;
    case 0xAD:
      instructions.push_back({&(c[i]), 1, "xor l"});
      break;
    case 0xAE:
      instructions.push_back({&(c[i]), 1, "xor (hl)"});
      break;
    case 0xAF:
      instructions.push_back({&(c[i]), 1, "xor a"});
      break;

    case 0xB0:
      instructions.push_back({&(c[i]), 1, "or b"});
      break;
    case 0xB1:
      instructions.push_back({&(c[i]), 1, "or c"});
      break;
    case 0xB2:
      instructions.push_back({&(c[i]), 1, "or d"});
      break;
    case 0xB3:
      instructions.push_back({&(c[i]), 1, "or e"});
      break;
    case 0xB4:
      instructions.push_back({&(c[i]), 1, "or h"});
      break;
    case 0xB5:
      instructions.push_back({&(c[i]), 1, "or l"});
      break;
    case 0xB6:
      instructions.push_back({&(c[i]), 1, "or (hl)"});
      break;
    case 0xB7:
      instructions.push_back({&(c[i]), 1, "or a"});
      break;
    case 0xB8:
      instructions.push_back({&(c[i]), 1, "cp b"});
      break;
    case 0xB9:
      instructions.push_back({&(c[i]), 1, "cp c"});
      break;
    case 0xBA:
      instructions.push_back({&(c[i]), 1, "cp d"});
      break;
    case 0xBB:
      instructions.push_back({&(c[i]), 1, "cp e"});
      break;
    case 0xBC:
      instructions.push_back({&(c[i]), 1, "cp h"});
      break;
    case 0xBD:
      instructions.push_back({&(c[i]), 1, "cp l"});
      break;
    case 0xBE:
      instructions.push_back({&(c[i]), 1, "cp (hl)"});
      break;
    case 0xBF:
      instructions.push_back({&(c[i]), 1, "cp a"});
      break;

    case 0xC0:
      instructions.push_back({&(c[i]), 1, "ret nz"});
      break;
    case 0xC1:
      instructions.push_back({&(c[i]), 1, "pop bc"});
      break;
    case 0xC2:
      instructions.push_back({&(c[i]), 3, "jp nz, a16"});
      add_branch(&branches, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xC3:
      instructions.push_back({&(c[i]), 3, "jp a16"});
      add_branch(&branches, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      end = true;
      break;
    case 0xC4:
      instructions.push_back({&(c[i]), 3, "call nz, a16"});
      add_function(&funcs, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xC5:
      instructions.push_back({&(c[i]), 1, "push bc"});
      break;
    case 0xC6:
      instructions.push_back({&(c[i]), 2, "add a, d8"});
      i++;
      break;
    case 0xC7:
      instructions.push_back({&(c[i]), 1, "rst 00h"});
      break;
    case 0xC8:
      instructions.push_back({&(c[i]), 1, "ret z"});
      break;
    case 0xC9:
      instructions.push_back({&(c[i]), 1, "ret"});
      end = true;
      break;
    case 0xCA:
      instructions.push_back({&(c[i]), 3, "jp z, a16"});
      add_branch(&branches, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xCB:
      instructions.push_back({&(c[i]), 1, "bitop (todo)"});
      break;
    case 0xCC:
      instructions.push_back({&(c[i]), 3, "call z, a16"});
      add_function(&funcs, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xCD:
      instructions.push_back({&(c[i]), 3, "call a16"});
      add_function(&funcs, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xCE:
      instructions.push_back({&(c[i]), 2, "adc a, d8"});
      i++;
      break;
    case 0xCF:
      instructions.push_back({&(c[i]), 1, "rst 08h"});
      break;

    case 0xD0:
      instructions.push_back({&(c[i]), 1, "ret nc"});
      break;
    case 0xD1:
      instructions.push_back({&(c[i]), 1, "pop hl"});
      break;
    case 0xD2:
      instructions.push_back({&(c[i]), 3, "jp nc, a16"});
      add_branch(&branches, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xD3:
      instructions.push_back({&(c[i]), 1, "ERROR"});
      break;
    case 0xD4:
      instructions.push_back({&(c[i]), 3, "call nc, a16"});
      add_function(&funcs, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    case 0xD5:
      instructions.push_back({&(c[i]), 1, "push de"});
      break;
    case 0xD6:
      instructions.push_back({&(c[i]), 2, "sub d8"});
      i++;
      break;
    case 0xD7:
      instructions.push_back({&(c[i]), 1, "rst 10h"});
      break;
    case 0xD8:
      instructions.push_back({&(c[i]), 1, "ret c"});
      break;
    case 0xD9:
      instructions.push_back({&(c[i]), 1, "reti"});
      break;
    case 0xDA:
      instructions.push_back({&(c[i]), 3, "jp c, a16"});
      add_branch(&branches, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    //    case 0xDB:
    case 0xDC:
      instructions.push_back({&(c[i]), 3, "call c, a16"});
      add_function(&funcs, ((uword_t)c[i + 1]) * 256 + (ubyte_t)c[i + 2]);
      i += 2;
      break;
    //    case 0xDD:
    case 0xDE:
      instructions.push_back({&(c[i]), 2, "sbc a, d8"});
      i++;
      break;
    case 0xDF:
      instructions.push_back({&(c[i]), 1, "rst 18h"});
      break;

    case 0xE0:
      instructions.push_back({&(c[i]), 2, "ldh (a8), a"});
      i++;
      break;
    case 0xE1:
      instructions.push_back({&(c[i]), 1, "pop hl"});
      break;
    case 0xE2:
      instructions.push_back({&(c[i]), 1, "ld (c), a"});
      break;
    //    case 0xE3:
    //    case 0xE4:
    case 0xE5:
      instructions.push_back({&(c[i]), 1, "push hl"});
      break;
    case 0xE6:
      instructions.push_back({&(c[i]), 2, "and d8"});
      i++;
      break;
    case 0xE7:
      instructions.push_back({&(c[i]), 1, "rst 20h"});
      break;
    case 0xE8:
      instructions.push_back({&(c[i]), 2, "add sp, r8"});
      i++;
      break;
    case 0xE9:
      instructions.push_back({&(c[i]), 1, "jp (hl)"});
      break;
    case 0xEA:
      instructions.push_back({&(c[i]), 3, "ld (a16), a"});
      i += 2;
      break;
    //    case 0xEB:
    //    case 0xEC:
    //    case 0xED:
    case 0xEE:
      instructions.push_back({&(c[i]), 2, "xor d8"});
      i++;
      break;
    case 0xEF:
      instructions.push_back({&(c[i]), 1, "rst 28h"});
      break;

    case 0xF0:
      instructions.push_back({&(c[i]), 2, "ldh a, (a8)"});
      i++;
      break;
    case 0xF1:
      instructions.push_back({&(c[i]), 1, "pop af"});
      break;
    case 0xF2:
      instructions.push_back({&(c[i]), 1, "ld a, (c)"});
      break;
    case 0xF3:
      instructions.push_back({&(c[i]), 1, "di"});
      break;
    //    case 0xF4:
    case 0xF5:
      instructions.push_back({&(c[i]), 1, "push af"});
      break;
    case 0xF6:
      instructions.push_back({&(c[i]), 2, "or d8"});
      i++;
      break;
    case 0xF7:
      instructions.push_back({&(c[i]), 1, "rst 30h"});
      break;
    case 0xF8:
      instructions.push_back({&(c[i]), 2, "ld hl, sp+r8"});
      i++;
      break;
    case 0xF9:
      instructions.push_back({&(c[i]), 1, "ld sp, hl"});
      break;
    case 0xFA:
      instructions.push_back({&(c[i]), 3, "ld a, (a16)"});
      i += 2;
      break;
    case 0xFB:
      instructions.push_back({&(c[i]), 1, "ei"});
      break;
    //    case 0xFC:
    //    case 0xFD:
    case 0xFE:
      instructions.push_back({&(c[i]), 2, "cp d8"});
      i++;
      break;
    case 0xFF:
      instructions.push_back({&(c[i]), 1, "rst 38h"});
      break;

    default:
      std::cout << "unknown opcode " << std::hex << (int)((ubyte_t)(c[i]))
                << std::endl;
      end = true;
      break;
    }

    i++;
    cnt++;

    if (instructions.size() != 0) {
      auto ins = instructions.at(instructions.size() - 1);
      std::cout << std::hex << "0x" << i - ins.sz << ": " << ins.str
                << std::endl;
    }

    if (end || i == 0xffff) {
      if (branch_i < branches.size()) {
        i = 0x100 + branches.at(branch_i++);
        std::cout << "New branch: " << std::hex << i << std::dec << std::endl;
      } else {
        if (func_i < funcs.size()) {
          i = 0x100 + funcs.at(func_i++);
          std::cout << "New function: " << std::hex << i << std::dec
                    << std::endl;
        } else
          break;
      }
    }

  } while (true);

  std::cout << instructions.size() << std::endl;
}

const gb_header_t *gb_header_t::fromFile(std::string filename) {
  std::ifstream ins(filename, std::ios::binary | std::ifstream::ate);

  int sz = ins.tellg();
  gb_cartridge_t cartridge = new char[sz];
  ins.seekg(0, std::ios::beg);
  ins.read((char *)cartridge, sz);

  gb_header_t *header = ((gb_header_t *)(&cartridge[0x100]));
  header->print();
  decode(cartridge, 0x100);
  return (gb_header_t *)(&cartridge[0x100]);
}
}
}
}
