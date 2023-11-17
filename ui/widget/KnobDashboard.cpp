//
// Created by 19254 on 2023/11/17.
//
#include <QPoint>
#include <QAction>
#include <cmath>
#include <QtMath>
#include <sstream>
#include <iomanip>
#include "KnobDashboard.hpp"

void KnobDashboard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    m_currentValue = 4.22f;  // 当前表盘读数
    m_minValue = 0;  // 表盘最小读数
    m_maxValue = 360;  // 表盘最大读数
    drawTicks(painter);
    drawBorder(painter);
    drawPointer(painter);

}

// 画刻度
void KnobDashboard::drawTicks(QPainter &painter) {
    QColor textColor(92, 92, 92);
    painter.setPen(textColor);

    auto po = CalculateText(painter,"零度偏移");
    painter.drawText(std::get<0>(po),std::get<1>(po)-13,"零度偏移");

    auto agent = getAngle();
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << agent << "°";
    po = CalculateText(painter,stream.str());
    painter.drawText(std::get<0>(po),std::get<1>(po)+13,stream.str().c_str());

}

// 画边框
void KnobDashboard::drawBorder(QPainter &painter) {
    painter.save();

    int borderWidth = 10;  // 修改边框厚度
    QRectF borderRect = rect().adjusted(borderWidth / 2, borderWidth / 2, -borderWidth / 2, -borderWidth / 2);

    QLinearGradient gradient(borderRect.topLeft(), borderRect.bottomRight());
    gradient.setColorAt(0, QColor("#3fd38f"));  // 起始颜色
    gradient.setColorAt(1, QColor("#50b4cb"));  // 终止颜色

    painter.setPen(QPen(gradient, borderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(borderRect);

    painter.restore();
}

// 画指针
void KnobDashboard::drawPointer(QPainter &painter) {
    painter.save();

    QPointF center(width() / 2.0, height() / 2.0);

    qreal angle = getAngle()-90;  // 标尺的角度按当前读数计算
    qreal radians = qDegreesToRadians(angle);

    int pointerShiftingStart = 65;
    int pointerShiftingEnd = 79;

// 计算矩形的两个端点坐标
    qreal x1 = center.x() + pointerShiftingStart * qCos(radians);
    qreal y1 = center.y() + pointerShiftingStart * qSin(radians);

    qreal x2 = center.x() + pointerShiftingEnd * qCos(radians);
    qreal y2 = center.y() + pointerShiftingEnd * qSin(radians);

    QLineF pointer(QPointF(x1, y1), QPointF(x2, y2));

    painter.setPen(QPen(Qt::black, 1));
    painter.drawLine(pointer);

    painter.restore();
}

double KnobDashboard::getAngle() const {
    return (m_currentValue / (m_maxValue - m_minValue) * 360.0);
}

std::tuple<int,int> KnobDashboard::CalculateText(QPainter &painter,const std::string& str) {
    // 获取组件的矩形区域
    QRect rect = this->rect();

    // 设置字体和文本
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);

    QString text = str.c_str();

    // 获取文本的大小
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(text);

    // 计算文本绘制的位置
    int x = (rect.width() - textRect.width()) / 2;
    int y = ((rect.height() - textRect.height()) / 2) + 14;
    return std::make_tuple(x,y);
}