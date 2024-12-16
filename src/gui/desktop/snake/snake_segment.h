#ifndef SNAKESEGMENT_H
#define SNAKESEGMENT_H

#include <QGraphicsItem>
#include <QPainter>
#include <iostream>
#include <utility>

#include "snake_model.h"

class snake_segment : public QGraphicsItem {
 public:
  explicit snake_segment(s21::snake_model::GameInfo_t const& gameInfo)
      : gameInfo(gameInfo) {};

 protected:
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override {
    painter->setBrush(Qt::gray);
    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 10; x++) {
        if (gameInfo.field[y][x] == s21::snake_model::Body)
          painter->drawEllipse(x * 40, y * 40, 40, 40);
        if (gameInfo.field[y][x] == s21::snake_model::Head)
          painter->drawRect(x * 40, y * 40, 40, 40);
        if (gameInfo.field[y][x] == s21::snake_model::Apple) {
          painter->setBrush(Qt::red);
          painter->drawRect(x * 40, y * 40, 40, 40);
          painter->setBrush(Qt::gray);
        }
      }
    }
  };
  QRectF boundingRect() const override { return {0, 0, 400, 800}; };
  s21::snake_model::GameInfo_t gameInfo;
};

#endif  // SNAKE_SEGMENT_H
