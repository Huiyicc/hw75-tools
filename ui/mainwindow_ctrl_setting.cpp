//
// Created by 19254 on 2023/12/4.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fmt/format.h"

void MainWindow::ctrlSettingInit(QWidget *parent) {
  connect(ui->ctrl_setting_sleep_time_horizontalSlider, &QSlider::valueChanged, this,
          &MainWindow::ctrlSettingEventCtrlSleepTimeValueChanged);
  ctrlSettingEventUpdataInfos();
}

void MainWindow::ctrlSettingEventUpdataInfos() {
  ui->ctrl_setting_sleep_time_groupBox->setTitle(
      fmt::format("休眠时间：{}秒", ui->ctrl_setting_sleep_time_horizontalSlider->value()).c_str());

};

bool ctrlSettingBreakValueChanged = false;

void MainWindow::ctrlSettingTabChanged() {
  Lib::HWDeviceTools tools;
  if (!checkCtrlConnect()) {
    std::cout << "未连接设备" << std::endl;
    return;
  }
  auto devices = getCtrlConnectDev();
  auto appCfg = tools.GetDynamicSysConf(devices);
  ctrlSettingBreakValueChanged = true;
  ui->ctrl_setting_sleep_time_horizontalSlider->setValue(appCfg.SleepTime);
  ctrlSettingEventUpdataInfos();
}

void MainWindow::ctrlSettingEventCtrlSleepTimeValueChanged(int value) {
  if (!ctrlSettingBreakValueChanged) {
    Lib::HWDeviceTools tools;
    if (!checkCtrlConnect()) {
      std::cout << "未连接设备" << std::endl;
      return;
    }
    Lib::CtrlSysCfg cfg;
    cfg.SleepTime = value;
    auto devices = getCtrlConnectDev();
    try {
      tools.SetDynamicSysConf(devices, cfg);
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
      exit(-1);
    }
  }
  ctrlSettingEventUpdataInfos();
  ctrlSettingBreakValueChanged = false;
}
