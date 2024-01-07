//
// Created by 19254 on 2023/9/27.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/HttpApis.hpp"

bool checkFirmwareVersion(int hostType=1);


// 初始化
void MainWindow::ctrlHomeInit(QWidget *parent) {
  // checkFirmwareVersion();
}


// 检查固件版本
bool checkFirmwareVersion(int hostType) {
  //utils::HttpApis::GetFirmwareLatestInfo(hostType);
  return false;
}