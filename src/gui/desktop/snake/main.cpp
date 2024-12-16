
#include <QThread>

#include "../../../controller/snake_controller.h"
#include "mainwindow.h"
#include "qapplication.h"

using namespace s21;
int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  snake_controller controller;
  MainWindow w;
  w.setController(&controller);
  w.show();
  return QApplication::exec();
}
