//
// Created by 19254 on 2023/9/27.
//
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "switchdialog.h"
#include "utils/config.hpp"

void MainWindow::ctrlInit(QWidget *parent) {
    connect(ui->systitle_button_connect, &QPushButton::clicked, this, &MainWindow::ctrlEventSwitchDevices);
    connect(ui->ctrl_eink_button_switchImage, &QPushButton::clicked, this, &MainWindow::ctrlEventSwitchImage);
    connect(ui->ctrl_eink_image_threshold_horizontalSlider, &QSlider::valueChanged, this,
            &MainWindow::ctrlEventEinkValueChanged);
    connect(ui->ctrl_eink_button_pushImage, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkPushImage);
    // 初始化控件
    ui->ctrl_groupBox_version->setVisible(false);
    ui->ctrl_eink_imgedit_group->setVisible(false);
    // 连接设备
    auto cfg = GetConfigInstance();
    auto ctrlPath = cfg->getConfig()["ctrl"]["path"].get<std::string>();
    if (!ctrlPath.empty()) {
        Lib::HWDeviceTools tools;
        std::vector<Lib::HWDevice> HWDevicesList;
        tools.GetHWDevicesList(HWDevicesList);
        for (int i = 0; i < HWDevicesList.size(); ++i) {
            if (HWDevicesList[i].Path == QString::fromStdString(ctrlPath)) {
                m_modelConnectStatus[HW_MODEL_NAME_CTRL] = HWDevicesList[i];
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
    for (int i = 0; i < HWDevicesList.size(); ++i) {
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
    cfg->getConfig()["ctrl"]["path"] = HWDevicesList[*result].Path.toStdString();
    cfg->saveConfig();
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

void MainWindow::ctrlEventSwitchImage(bool checked) {
    // 选择图片按钮事件
    // 创建文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "图像文件 (*.jpg *.jpeg *.png *.bmp)");;
    // 检查用户是否选择了文件
    ui->ctrl_eink_button_switchImage->setVisible(!filePath.isEmpty());
    if (filePath.isEmpty()) {
        return;
    }
    ui->ctrl_eink_imgview->clear();
    ui->ctrl_eink_edit_imagePath->setText(filePath);
    // 设置预览图像
    auto tmpptr = std::make_shared<Lib::Image>(filePath);
    m_userPushImage = tmpptr;
    auto img = m_userPushImage->GetImage();
    ui->ctrl_eink_image_threshold_horizontalSlider->setMinimum(1);
    ui->ctrl_eink_image_threshold_horizontalSlider->setMaximum(m_userPushImage->GetMaximumThreshold());
    ui->ctrl_eink_image_threshold_label->setText("50");
    ui->ctrl_eink_image_threshold_horizontalSlider->setValue(50);
    ui->ctrl_eink_imgedit_group->setEnabled(true);
    ui->ctrl_eink_imgedit_group->setVisible(true);
    ctrlEventEinkValueChanged(50);
}

void MainWindow::ctrlEventEinkValueChanged(int value) {
    // 阈值条拖动之后事件
    ui->ctrl_eink_image_threshold_label->setText(QString::number(value));
    ctrlEinkUpdateUserPreviewImage();
}

void MainWindow::ctrlEinkUpdateUserPreviewImage() {
    QImage scaledimg;
    std::shared_ptr<QImage> img = m_userPushImage->GetBinaryImgData(ui->ctrl_eink_image_threshold_horizontalSlider->value());
    scaledimg = img->scaled(ui->ctrl_eink_imgview->width(), ui->ctrl_eink_imgview->height(), Qt::KeepAspectRatio);
    ui->ctrl_eink_imgview->setPixmap(QPixmap::fromImage(scaledimg));
}

void MainWindow::ctrlEventEinkPushImage(bool checked) {
    // 更新屏幕图片按钮事件
    try {
        if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL)==m_modelConnectStatus.end()) {
            return;
        }
        ui->ctrl_eink_button_pushImage->setEnabled(false);
        ui->ctrl_eink_button_pushImage->setText("推送中");
        Lib::HWDeviceTools tools;

        tools.SetDynamicScerrn(m_modelConnectStatus[HW_MODEL_NAME_CTRL], *m_userPushImage->BinaryImgDataToBits(ui->ctrl_eink_image_threshold_horizontalSlider->value()));
        QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
    }catch (Lib::DeviceException &e) {
        QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    } catch (std::exception &e) {
        QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    }
    ui->ctrl_eink_button_pushImage->setEnabled(true);
    ui->ctrl_eink_button_pushImage->setText("推送图片");
}