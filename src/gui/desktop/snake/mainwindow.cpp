#include "mainwindow.h"

#include <ui_mainwindow.h>

#include <QDir>
#include <QThread>
#include <iostream>

#include "snake_segment.h"
MainWindow::MainWindow(QWidget* parent, snake_segment* segment,
                       s21::snake_controller* controller)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      segment(nullptr),
      controller(nullptr) {
  ui->setupUi(this);

  ui->graphicsView->setFocusPolicy(
      Qt::StrongFocus);  // Сильный фокус на QGraphicsView
  this->setFocusPolicy(Qt::StrongFocus);  // Сильный фокус на главное окно

  ui->graphicsView->installEventFilter(this);

  scene = new QGraphicsScene(this);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  scene->setSceneRect(0, 0, 400, 800);
  QPixmap pixmap;
  scene->addRect(0, 0, 400, 800);
  if (!pixmap.load("misc/images/grass.png")) {
    std::cerr << "Failed to load background image" << std::endl;
  } else
    ui->graphicsView->setBackgroundBrush(pixmap.scaled(50, 50));

  keyMap = QMultiMap<Qt::Key, s21::snake_model::UserAction_t>();
  keyMap.insert(Qt::Key_Space, s21::snake_model::Start);
  keyMap.insert(Qt::Key_P, s21::snake_model::Pause);
  keyMap.insert(Qt::Key_Escape, s21::snake_model::Terminate);
  keyMap.insert(Qt::Key_Left, s21::snake_model::Left);
  keyMap.insert(Qt::Key_A, s21::snake_model::Left);
  keyMap.insert(Qt::Key_Right, s21::snake_model::Right);
  keyMap.insert(Qt::Key_D, s21::snake_model::Right);
  keyMap.insert(Qt::Key_Up, s21::snake_model::Up);
  keyMap.insert(Qt::Key_W, s21::snake_model::Up);
  keyMap.insert(Qt::Key_Down, s21::snake_model::Down);
  keyMap.insert(Qt::Key_S, s21::snake_model::Down);
  keyMap.insert(Qt::Key_Shift, s21::snake_model::Action);
  connect(this, SIGNAL(triggerUpdateScene(s21::snake_model::GameInfo_t)), this,
          SLOT(updateScene(s21::snake_model::GameInfo_t)));
  QThread* draw_thread = QThread::create([this] { drawGame(); });
  draw_thread->start();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::drawGame() {
  s21::snake_model::GameInfo_t game_info;
  while (this->window()->isEnabled()) {
    QThread::msleep(77);
    game_info = controller->updateCurrentState();
    emit triggerUpdateScene(game_info);
    ui->Score->setText("Score: " + QString::number(game_info.score));
    ui->BestScore->setText("Best score: " +
                           QString::number(game_info.high_score));
    ui->Level->setText("Level: " + QString::number(game_info.level));
    ui->Speed->setText("Speed: " + QString::number(game_info.level * 5));
  }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  if (keyMap.contains(static_cast<const Qt::Key>(event->key()))) {
    controller->userInput(
        keyMap.value(static_cast<const Qt::Key>(event->key())),
        event->modifiers() == Qt::ShiftModifier);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Shift) {
    controller->userInput(
        keyMap.value(static_cast<const Qt::Key>(event->key())),
        event->modifiers() == Qt::ShiftModifier);
  }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
  QKeyEvent* keyEvent = nullptr;
  bool result = false;

  if (event->type() == QEvent::KeyPress) {
    keyEvent = dynamic_cast<QKeyEvent*>(event);
    this->keyPressEvent(keyEvent);
    result = true;
  }

  else if (event->type() == QEvent::KeyRelease) {
    keyEvent = dynamic_cast<QKeyEvent*>(event);
    this->keyReleaseEvent(keyEvent);
    result = true;
  }

  else
    result = QObject::eventFilter(obj, event);

  return result;
}
void MainWindow::setController(s21::snake_controller* c) { controller = c; }

void MainWindow::updateScene(s21::snake_model::GameInfo_t game_info) {
  if (segment) {
    if (segment->scene() == scene) {
      scene->removeItem(segment);
    }
    delete segment;
  }
  segment = new snake_segment(game_info);
  scene->addItem(segment);
  scene->update();
}
