//
// Created by alex on 9/14/24.
//

#include <QGraphicsItem>
#include <QPainter>

#include "../../brick_game/tetris/tetris.h"

#ifndef CPP3_GAMES_TETRIS_PAINTER_H
#define CPP3_GAMES_TETRIS_PAINTER_H
class tetris_painter : public QGraphicsItem {
 public:
  explicit tetris_painter(GameInfo_t* game) : gameInfo(game) {};

 protected:
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override {
    painter->setBrush(Qt::red);
    position pos;

    for (int y = 0; y < 22; y++) {
      for (int x = 0; x < 10; x++) {
        if (get_cell(gameInfo, y, x) != types_of_cells::EMPTY_CELL) {
          painter->drawRect(x * 40, y * 40, 40, 40);
        }
      }
    }

    for (int i = 0; i < 4; i++) {
      pos = TETRIS_BRICK[gameInfo->next_tetromino.type]
                        [gameInfo->next_tetromino.orientation][i];
    }
  };
  QRectF boundingRect() const override { return {0, 0, 400, 900}; };
  GameInfo_t* gameInfo;
};

class tetromino_painter : public QGraphicsItem {
 public:
  explicit tetromino_painter(GameInfo_t* game) : gameInfo(game) {};

 protected:
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override {
    painter->setBrush(Qt::red);
    position pos;
    for (int i = 0; i < 4; i++) {
      pos = TETRIS_BRICK[gameInfo->next_tetromino.type]
                        [gameInfo->next_tetromino.orientation][i];
      painter->drawRect(pos.x * 40, pos.y * 40, 40, 40);
    }
  };
  QRectF boundingRect() const override { return {0, 0, 40, 40}; };
  GameInfo_t* gameInfo;
};

#endif  // CPP3_GAMES_TETRIS_PAINTER_H
