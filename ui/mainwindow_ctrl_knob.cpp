//
// Created by 19254 on 2023/11/17.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "knobaligndialog.h"

void MainWindow::knobInit(QWidget *parent) {
    connect(ui->ctrl_knob_button_calibration, &QPushButton::clicked, this, &MainWindow::knobEventCalibration);
}

void MainWindow::knobEventCalibration(bool checked) {
    auto pDialog = new KnobAlignDialog(this);
    pDialog->exec();
    delete pDialog;
}