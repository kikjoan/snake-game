#include "mainwindowtetris.h"

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QThread>

#include "tetris_painter.h"
#include "ui_mainwindowtetris.h"

MainWindowTetris::MainWindowTetris(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowTetris) {
  ui->setupUi(this);
  ui->graphicsView->setFocusPolicy(
      Qt::StrongFocus);  // Сильный фокус на QGraphicsView
  this->setFocusPolicy(Qt::StrongFocus);  // Сильный фокус на главное окно

  ui->graphicsView->installEventFilter(this);

  gameScene = new QGraphicsScene(this);
  ui->graphicsView->setScene(gameScene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  gameScene->setSceneRect(0, 0, 403, 880);
  gameScene->addRect(0, 0, 403, 880);

  nextTetrominoScene = new QGraphicsScene(this);
  ui->nextTetromino->setScene(nextTetrominoScene);
  ui->nextTetromino->setRenderHint(QPainter::Antialiasing);
  ui->nextTetromino->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->nextTetromino->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->nextTetromino->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  keyMap.insert(Qt::Key_Down, Down);
  keyMap.insert(Qt::Key_Left, Left);
  keyMap.insert(Qt::Key_Right, Right);
  keyMap.insert(Qt::Key_Space, Action);
  keyMap.insert(Qt::Key_P, Pause);
  keyMap.insert(Qt::Key_Q, UserAction_t::Terminate);
  game = create_game(HEIGHT + 2, WIDTH);

  connect(this, &MainWindowTetris::triggerUpdateScene, this,
          &MainWindowTetris::updateScene);
  QThread *tetrisThread = QThread::create(&MainWindowTetris::gameLoop, this);
  tetrisThread->start();
}

void MainWindowTetris::gameLoop() {
  bool game_over = true;
  movement = Start;
  while (game_over) {
    QThread::msleep(game->speed);
    emit triggerUpdateScene(game);
    if (movement != Pause) game_over = tick(game, movement);
  }
  movement = Terminate;
  emit triggerUpdateScene(game);
  // TODO: Add game over screen in to graphics view
}

MainWindowTetris::~MainWindowTetris() { delete ui; }

void MainWindowTetris::updateScene(GameInfo_t *game_info) {
  if (tetrisPainter) {
    if (tetrisPainter->scene() == gameScene) {
      gameScene->removeItem(tetrisPainter);
    }
    delete tetrisPainter;
  }
  tetrisPainter = new tetris_painter(game_info);
  gameScene->addItem(tetrisPainter);
  gameScene->update();
  if (nextTetrominoPainter) {
    if (nextTetrominoPainter->scene() == nextTetrominoScene) {
      nextTetrominoScene->removeItem(nextTetrominoPainter);
    }
    delete nextTetrominoPainter;
  }
  nextTetrominoPainter = new tetromino_painter(game_info);
  nextTetrominoScene->addItem(nextTetrominoPainter);
  nextTetrominoScene->update();

  ui->Score->setText("Score: " + QString::number(game_info->score));
  ui->BestScore->setText("Best score: " +
                         QString::number(game_info->high_score));
  if (movement == 2) {
    QGraphicsTextItem *gameOverText = new QGraphicsTextItem(
        "Game Over\n SCORE: " + QString::number(game_info->score) +
        "\n Press Q to exit");
    QFont font("Arial", 40, QFont::Bold);
    gameOverText->setFont(font);
    gameOverText->setDefaultTextColor(Qt::blue);
    QRectF sceneRect = gameScene->sceneRect();
    qreal x = (sceneRect.width() - gameOverText->boundingRect().width()) / 2;
    qreal y = (sceneRect.height() - gameOverText->boundingRect().height()) / 2;
    gameOverText->setPos(x, y);
    gameScene->addItem(gameOverText);
    gameScene->update();
  }
}

void MainWindowTetris::keyPressEvent(QKeyEvent *event) {
  if (keyMap.contains(static_cast<const Qt::Key>(event->key())) &&
      movement != Pause)
    tick(game, keyMap.value(static_cast<Qt::Key>(event->key())));
  if (static_cast<const Qt::Key>(event->key()) == Qt::Key_P)
    if (movement == Pause)
      movement = Start;
    else
      movement = Pause;
  else if (static_cast<const Qt::Key>(event->key()) == Qt::Key_Escape ||
           static_cast<const Qt::Key>(event->key()) == Qt::Key_Q) {
    this->close();
  }
}

bool MainWindowTetris::eventFilter(QObject *obj, QEvent *event) {
  QKeyEvent *keyEvent = nullptr;
  bool result = false;

  if (event->type() == QEvent::KeyPress) {
    keyEvent = dynamic_cast<QKeyEvent *>(event);
    this->keyPressEvent(keyEvent);
    result = true;
  }

  else if (event->type() == QEvent::KeyRelease) {
    keyEvent = dynamic_cast<QKeyEvent *>(event);
    this->keyReleaseEvent(keyEvent);
    result = true;
  }

  else
    result = QObject::eventFilter(obj, event);

  return result;
}
