#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMouseEvent>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // 设置无边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    ctrlInit(parent);
    ctrlEinkInit(parent);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    QPoint y = event->globalPos();
    QPoint x = y - z;
    this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    QPoint x = this->geometry().topLeft();
    QPoint y = event->globalPos();
    z = y - x;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}
