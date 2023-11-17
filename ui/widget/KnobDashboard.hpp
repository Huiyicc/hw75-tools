//
// Created by 19254 on 2023/11/17.
//

#ifndef HW_TOOLS_KNOBDASHBOARD_HPP
#define HW_TOOLS_KNOBDASHBOARD_HPP

#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QPen>
#include <QBrush>
#include <iostream>
#include <QTimer>

namespace Ui {
    class KnobDashboard;
};

// 电机标尺仪表盘
class KnobDashboard : public QWidget {
public:

    explicit KnobDashboard(QWidget *parent = nullptr) : QWidget(parent), m_angle(0) {

    }
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawTicks(QPainter &painter);

    void drawBorder(QPainter &painter);

    void drawPointer(QPainter &painter);

    double getAngle() const;
    std::tuple<int,int> CalculateText(QPainter &painter,const std::string& str);

private:
    float m_angle = 0;
    float m_currentValue = 0;  // 当前表盘读数
    float m_minValue = 0;  // 表盘最小读数
    float m_maxValue = 12;  // 表盘最大读数
};


#endif //HW_TOOLS_KNOBDASHBOARD_HPP
