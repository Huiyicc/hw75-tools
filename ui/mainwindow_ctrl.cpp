//
// Created by 19254 on 2023/9/27.
//
#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "ui/switchdialog.h"
#include "utils/Log.hpp"
#include "utils/config.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <thread>

void MainWindow::ctrlInit(QWidget *parent) {
  connect(ui->systitle_button_connect, &QPushButton::clicked, this, &MainWindow::ctrlEventSwitchDevices);
  // 初始化控件
  ui->ctrl_groupBox_version->setVisible(false);
  // 连接设备
  auto cfg = GetConfigInstance();
  auto ctrlPath = cfg->Ctrl.Path;
  if (!ctrlPath.empty()) {
    Lib::HWDeviceTools tools;
    std::vector<Lib::HWDevice> HWDevicesList;
    tools.GetHWDevicesList(HWDevicesList);
    for (auto &i: HWDevicesList) {
      if (i.Path == QString::fromStdString(ctrlPath)) {
        m_modelConnectStatus[HW_MODEL_NAME_CTRL] = i;
        break;
      }
    }
  }
  ctrlEventConnectDevices();
}

void MainWindow::ctrlEventSwitchDevices(bool checked) {
  if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) != m_modelConnectStatus.end()) {
    // 已连接
    ui->systitle_button_connect->setText("连接设备");
    ui->ctrl_tabWidget->setEnabled(false);
    ui->ctrl_groupBox_version->setVisible(false);
    m_modelConnectStatus.erase(HW_MODEL_NAME_CTRL);
    return;
  }
  std::map<std::string, int> data;
  std::vector<Lib::HWDevice> HWDevicesList;
  Lib::HWDeviceTools tools;
  tools.GetHWDevicesList(HWDevicesList);
  for (size_t i = 0; i < HWDevicesList.size(); ++i) {
    data[HWDevicesList[i].ProductName.toStdString()] = i;
  }
  std::shared_ptr<int> result = std::make_shared<int>(-1);
  auto pDialog = new SwitchDialog(data, result, this);
  pDialog->exec();
  delete pDialog;
  if (*result == -1) {
    return;
  }
  m_modelConnectStatus[HW_MODEL_NAME_CTRL] = HWDevicesList[*result];
  auto cfg = GetConfigInstance();
  cfg->Ctrl.Path = HWDevicesList[*result].Path.toStdString();
  cfg.saveConfig();
  ctrlEventConnectDevices();
}

std::shared_ptr<std::thread> ctrlConnectTickThread = nullptr;

void MainWindow::ctrlEventConnectDevices() {
  if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) { return; }
  Lib::HWDeviceTools tools;
  auto ver = tools.GetDynamicVersion(m_modelConnectStatus[HW_MODEL_NAME_CTRL]);
  ui->systitle_button_connect->setText("断开连接");
  ui->ctrl_tabWidget->setEnabled(true);
  ui->ctrl_groupBox_version->setVisible(true);
  ui->ctrl_version_label_branch->setText(ver.GitBranch);
  ui->ctrl_version_label_hash->setText(ver.GitHash);
  ui->ctrl_version_label_build_version->setText(ver.BuildVersion);
  if (ctrlConnectTickThread == nullptr) {
    // 插件tick事件
    ctrlConnectTickThread = std::make_shared<std::thread>([this]() {
      while (true) {
        if (g_mainWindow == nullptr) {
          continue;
        }
        PrintDebug("扩展连接Tick: {}", std::to_string(g_mainWindow->checkCtrlConnect()));
        if (!g_mainWindow->checkCtrlConnect()) {
          // 未连接设备
          // 断开设备
          if (ui->systitle_button_connect->text() != "连接设备") {
            ui->systitle_button_connect->setText("连接设备");
            ui->ctrl_tabWidget->setEnabled(false);
            ui->ctrl_groupBox_version->setVisible(false);
            m_modelConnectStatus.erase(HW_MODEL_NAME_CTRL);
          }
          // 延迟1秒
          std::this_thread::sleep_for(std::chrono::seconds(1));
          continue;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });
  }

}