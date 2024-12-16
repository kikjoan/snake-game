#ifndef MAINWINDOWTETRIS_H
#define MAINWINDOWTETRIS_H

#include <QGraphicsScene>
#include <QMainWindow>

#include "../../brick_game/tetris/tetris.h"
#include "tetris_painter.h"

namespace Ui {
class MainWindowTetris;
}

class MainWindowTetris : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindowTetris(QWidget* parent = nullptr);
  ~MainWindowTetris();
  bool eventFilter(QObject* obj, QEvent* event) override;

 protected:
  void gameLoop();

 public slots:
  void updateScene(GameInfo_t* game_info);

 signals:
  void triggerUpdateScene(GameInfo_t* game_info);

 private:
  UserAction_t movement;
  GameInfo_t* game;
  Ui::MainWindowTetris* ui;
  QGraphicsScene* gameScene;
  QGraphicsScene* nextTetrominoScene;
  tetris_painter* tetrisPainter{};
  tetromino_painter* nextTetrominoPainter{};
  QMultiMap<Qt::Key, UserAction_t> keyMap = QMultiMap<Qt::Key, UserAction_t>();

 protected:
  void keyPressEvent(QKeyEvent* event) override;
};

#endif  // MAINWINDOWTETRIS_H
