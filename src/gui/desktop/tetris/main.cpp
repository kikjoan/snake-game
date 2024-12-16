//
// Created by alex on 9/5/24.
//

#include "mainwindowtetris.h"
#include "qapplication.h"
int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindowTetris w;
  w.show();
  return QApplication::exec();
}