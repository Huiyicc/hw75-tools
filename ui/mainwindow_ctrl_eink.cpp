//
// Created by 19254 on 2023/9/27.
//
#include "mainwindow.h"
#include "./ui_mainwindow.h"

// 初始化
void MainWindow::ctrlEinkInit(QWidget *parent) {
    connect(ui->ctrl_eink_button_switchImage, &QPushButton::clicked, this, &MainWindow::ctrlEinkEventSwitchImage);

}

// 选择图片
void MainWindow::ctrlEinkEventSwitchImage(bool checked) {

}

