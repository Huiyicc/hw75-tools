//
// Created by 19254 on 2023/10/10.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "utils/process.hpp"
#include "QThread"


void MainWindow::ctrlOLEDInit(QWidget *parent) {
    connect(ui->ctrl_oled_button_switchImage, &QPushButton::clicked, this, &MainWindow::ctrlEventOLEDSwitchImage);
    connect(ui->ctrl_oled_image_threshold_horizontalSlider, &QSlider::valueChanged, this,
            &MainWindow::ctrlEventOLEDValueChanged);
    connect(ui->ctrl_oled_button_pushImage, &QPushButton::clicked, this, &MainWindow::ctrlEventOLEDPushImage);
    ui->ctrl_oled_imgedit_group->setVisible(false);
}

void MainWindow::ctrlEventOLEDSwitchImage(bool checked) {
    // 选择图片按钮事件
    // 创建文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "图像文件 (*.jpg *.jpeg *.png *.bmp)");;
    // 检查用户是否选择了文件
    ui->ctrl_oled_button_switchImage->setVisible(!filePath.isEmpty());
    if (filePath.isEmpty()) {
        return;
    }
    ui->ctrl_oled_imgedit_group->setVisible(true);
    ui->ctrl_oled_imgview->clear();
    ui->ctrl_oled_edit_imagePath->setText(filePath);
    // 设置预览图像
    auto tmpptr = std::make_shared<Lib::Image>(filePath);
    m_userPushImage = tmpptr;
    auto img = m_userPushImage->GetImage();
    ui->ctrl_oled_image_threshold_horizontalSlider->setMinimum(1);
    ui->ctrl_oled_image_threshold_horizontalSlider->setMaximum(m_userPushImage->GetMaximumThreshold());
    ui->ctrl_oled_image_threshold_label->setText("50");
    ui->ctrl_oled_image_threshold_horizontalSlider->setValue(50);
    ui->ctrl_oled_imgedit_group->setEnabled(true);
    ui->ctrl_oled_imgedit_group->setVisible(true);
    ctrlEventOLEDValueChanged(50);
}

void MainWindow::ctrlEventOLEDValueChanged(int value) {
    // 阈值条拖动之后事件
    ui->ctrl_oled_image_threshold_label->setText(QString::number(value));
    ctrlOLEDUpdateUserPreviewImage();
}

void MainWindow::ctrlEventOLEDPushImage(bool checked) {
    // 更新屏幕图片按钮事件
    try {
        if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) {
            return;
        }
        ui->ctrl_oled_button_pushImage->setEnabled(false);
        ui->ctrl_oled_button_pushImage->setText("推送中");

//        {
//            std::map<std::string, uint64_t> processList;
//            Lib::ProcessList(processList);
//            while (processList.find("YuanShen.exe")==processList.end()) {
//                Lib::ProcessList(processList);
//                QThread::sleep(1);
//            }
//        }
//        QThread::sleep(6);
//        std::cout<<"YuanShen.exe"<<std::endl;

        Lib::HWDeviceTools tools;

        tools.SetDynamicOLEDScerrn(m_modelConnectStatus[HW_MODEL_NAME_CTRL], *m_userPushImage->BinaryImgDataToOLEDBits(
                ui->ctrl_oled_image_threshold_horizontalSlider->value(),true));
//        QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
    } catch (Lib::DeviceException &e) {
        QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    } catch (std::exception &e) {
        QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    }
    ui->ctrl_oled_button_pushImage->setEnabled(true);
    ui->ctrl_oled_button_pushImage->setText("推送图片");
}

void MainWindow::ctrlOLEDUpdateUserPreviewImage() {
    QImage scaledimg;
    std::shared_ptr<QImage> img = m_userPushImage->GetBinaryImgData(
            ui->ctrl_oled_image_threshold_horizontalSlider->value(), 32, 128);
    scaledimg = img->scaled(ui->ctrl_oled_imgview->width(), ui->ctrl_oled_imgview->height(), Qt::KeepAspectRatio);
    ui->ctrl_oled_imgview->setPixmap(QPixmap::fromImage(scaledimg));
}

