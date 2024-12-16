#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMainWindow>

#include "../../../controller/snake_controller.h"
#include "snake_model.h"
#include "snake_segment.h"

namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr,
                      snake_segment* segment = nullptr,
                      s21::snake_controller* controller = nullptr);
  ~MainWindow() override;
  void setController(s21::snake_controller* c);
  bool eventFilter(QObject* obj, QEvent* event) override;

  void drawGame();

 private:
  Ui::MainWindow* ui;
  QGraphicsScene* scene;
  snake_segment* segment;
  s21::snake_controller* controller;
  QMultiMap<Qt::Key, s21::snake_model::UserAction_t> keyMap;

 public slots:
  void updateScene(s21::snake_model::GameInfo_t game_info);
 signals:
  void triggerUpdateScene(s21::snake_model::GameInfo_t game_info);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
};

#endif  // MAINWINDOW_H
