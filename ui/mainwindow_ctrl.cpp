//
// Created by 19254 on 2023/9/27.
//
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/switchdialog.h"
#include "utils/config.hpp"

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
        for (auto & i : HWDevicesList) {
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

void MainWindow::ctrlEventConnectDevices() {
    if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) { return; }
    Lib::HWDeviceTools tools;
    auto ver = tools.GetDynamicVersion(m_modelConnectStatus[HW_MODEL_NAME_CTRL]);
    ui->systitle_button_connect->setText("断开连接");
    ui->ctrl_tabWidget->setEnabled(true);
    ui->ctrl_groupBox_version->setVisible(true);
    ui->ctrl_version_label_branch->setText(ver.GitBranch);
    ui->ctrl_version_label_hash->setText(ver.GitHash);
}