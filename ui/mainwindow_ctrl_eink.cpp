//
// Created by 19254 on 2023/9/27.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

// 初始化
void MainWindow::ctrlEinkInit(QWidget *parent) {
	connect(ui->ctrl_eink_button_switchImage, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkSwitchImage);
	connect(ui->ctrl_eink_image_threshold_horizontalSlider, &QSlider::valueChanged, this,
	        &MainWindow::ctrlEventEinkValueChanged);
	connect(ui->ctrl_eink_button_pushImage, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkPushImage);
	ui->ctrl_eink_imgedit_group->setVisible(false);
}

// 选择图片
void MainWindow::ctrlEventEinkSwitchImage(bool checked) {
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

void MainWindow::ctrlEinkUpdateUserPreviewImage() {
	QImage scaledimg;
	std::shared_ptr<QImage> img = m_userPushImage->GetBinaryImgData(
			ui->ctrl_eink_image_threshold_horizontalSlider->value());
	scaledimg = img->scaled(ui->ctrl_eink_imgview->width(), ui->ctrl_eink_imgview->height(), Qt::KeepAspectRatio);
	ui->ctrl_eink_imgview->setPixmap(QPixmap::fromImage(scaledimg));
}

void MainWindow::ctrlEventEinkValueChanged(int value) {
	// 阈值条拖动之后事件
	ui->ctrl_eink_image_threshold_label->setText(QString::number(value));
	ctrlEinkUpdateUserPreviewImage();
}

void MainWindow::ctrlEventEinkPushImage(bool checked) {
	// 更新屏幕图片按钮事件
	try {
		if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) {
			return;
		}
		ui->ctrl_eink_button_pushImage->setEnabled(false);
		ui->ctrl_eink_button_pushImage->setText("推送中");
		Lib::HWDeviceTools tools;

		tools.SetDynamicEinkScerrn(m_modelConnectStatus[HW_MODEL_NAME_CTRL], *m_userPushImage->BinaryImgDataToBits(
				ui->ctrl_eink_image_threshold_horizontalSlider->value()));
		QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
	} catch (Lib::DeviceException &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
	} catch (std::exception &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
	}
	ui->ctrl_eink_button_pushImage->setEnabled(true);
	ui->ctrl_eink_button_pushImage->setText("推送图片");
}

bool MainWindow::ctrlEinkPushImage(const char *data, int len) {
	try {
		if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) {
			// TODO: 未连接设备
			return false;
		}
		// 创建图像对象
		QByteArray byteArray(data, len);
		auto imgLib = Lib::Image(QImage::fromData(byteArray));
		Lib::HWDeviceTools tools;
		tools.SetDynamicEinkScerrn(m_modelConnectStatus[HW_MODEL_NAME_CTRL], *imgLib.BinaryImgDataToBits(155));
		// QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
		return true;
	} catch (Lib::DeviceException &e) {
		// QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
	} catch (std::exception &e) {
		// QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
	}
	return false;
}