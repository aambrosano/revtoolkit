#include "mainwindow.h"
#include "archives/gameboy/gb.h"

namespace gb = rev::archives::gameboy;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  gb::gb_header_t::fromFile(
      "/home/alessandro/Downloads/Pokemon R.B.Y.G.S.C/Pokemon Gold.gbc");
  //  gb::gb_header_t::fromFile(
  //      "/home/alessandro/Downloads/Pokemon R.B.Y.G.S.C/Pokemon Blue.gb");
}

MainWindow::~MainWindow() {}
