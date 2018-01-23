TEMPLATE = lib

BASE_DIR = cpu

HEADERS += $$BASE_DIR/x86/instructions.h \
    $$BASE_DIR/c64/instructions.h \
    $$BASE_DIR/arm/instructions.h \
    $$BASE_DIR/sparc/instructions.h \
    $$BASE_DIR/z80/instructions.h \
    $$BASE_DIR/power/instructions.h
