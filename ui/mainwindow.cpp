#include <QMessageBox>
#include <QFileDialog>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::eventRegister() {
	connect(ui->comboBox_motor_equipment, QOverload<int>::of(&QComboBox::currentIndexChanged),
	        this, &MainWindow::eventByDynamicComboxUpdateTable);
	connect(ui->elink_switch_image, &QPushButton::clicked, this, &MainWindow::eventByElinkSwitchImage);
	connect(ui->pushButton_push_image, &QPushButton::clicked, this, &MainWindow::eventByPushImage);
	connect(ui->threshold_progress_horizontalSlider, &QSlider::valueChanged, this,
	        &MainWindow::eventByValueChanged);
	// 事件: 开启/关闭 二值化图像
	connect(ui->checkBox_tw_img, &QCheckBox::stateChanged, this, [&](int c) {
		ui->groupBox_4->setEnabled(c != 0);
		updateUserPreviewImage();
	});
}

void MainWindow::showEvent(QShowEvent *event) {
	try {
		ui->groupbox_set_image->setVisible(false);
		eventRegister();
		std::vector<Lib::HWDevice> HWDevicesList;
		Lib::HWDeviceTools tools;
		tools.GetHWDevicesList(HWDevicesList);
		// 清空设备列表
		ui->comboBox_motor_equipment->clear();
		// 遍历设备列表
		for (auto &HWDevice: HWDevicesList) {
			QMetaType::fromType<Lib::HWDevice>();
			QVariant v;
			v.setValue(HWDevice);
			// 添加设备到列表
			ui->comboBox_motor_equipment->addItem(HWDevice.ProductName, v);
		}
		// 如果没有设备
		if (HWDevicesList.empty()) {
			// 禁用按钮
			ui->electrical_machinery_contenct->setEnabled(false);
			QMainWindow::showEvent(event);
			return;
		}
		// 启用按钮
		ui->electrical_machinery_contenct->setEnabled(true);
		m_devices = HWDevicesList[0];
		updateDynamicLabel();
	} catch (Lib::DeviceException &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
		return;
	} catch (std::exception &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
		return;
	}
	// 调用父类方法
	QMainWindow::showEvent(event);
}

void MainWindow::eventByDynamicComboxUpdateTable(int index) {
	// 电机设备下拉框事件
	// 更新缓存设备信息
	m_devices = ui->comboBox_motor_equipment->itemData(index).value<Lib::HWDevice>();
}

void MainWindow::eventByElinkSwitchImage(bool checked) {
	// 选择图片按钮事件
	// 创建文件选择对话框
	QString filePath = QFileDialog::getOpenFileName(nullptr, "选择图片", "", "图像文件 (*.jpg *.jpeg *.png *.bmp)");;
	// 检查用户是否选择了文件
	ui->groupbox_set_image->setVisible(!filePath.isEmpty());
	if (filePath.isEmpty()) {
		return;
	}
	ui->elink_image_path->setText(filePath);
	// 设置预览图像
	auto tmpptr = std::make_shared<Lib::Image>(filePath);
	m_userPushImage = tmpptr;
	auto img = m_userPushImage->GetImage();
	ui->threshold_progress_horizontalSlider->setMaximum(1);
	ui->threshold_progress_horizontalSlider->setMaximum(m_userPushImage->GetMaximumThreshold());
	ui->threshold_progress_label->setText("50");
	ui->threshold_progress_horizontalSlider->setValue(50);
	ui->checkBox_tw_img->setChecked(true);
	ui->groupBox_4->setEnabled(true);
	eventByValueChanged(50);
}

void MainWindow::eventByPushImage(bool checked) {
	// 更新屏幕图片按钮事件
	try {
		Lib::HWDeviceTools tools;

		tools.SetDynamicScerrn(m_devices, *m_userPushImage->BinaryImgDataToBits(ui->checkBox_tw_img->checkState() > 0
		                                                                       ? ui->threshold_progress_horizontalSlider->value()
		                                                                       : -1));


	}catch (Lib::DeviceException &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
		return;
	} catch (std::exception &e) {
		QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
		return;
	}
}

void MainWindow::eventByValueChanged(int value) {
	// 阈值条拖动之后事件
	ui->threshold_progress_label->setText(QString::number(value));
	updateUserPreviewImage();

}

void MainWindow::updateDynamicLabel() {
	Lib::HWDeviceTools tools;
	auto version = tools.GetDynamicVersion(m_devices);
	ui->app_version_lable->setText(version.AppVersion);
	ui->zmk_version_lable->setText(version.ZmkVersion);
	ui->zephyr_version_lable->setText(version.ZephyrVersion);
}

void MainWindow::updateUserPreviewImage() {
	QImage scaledimg;
	std::shared_ptr<QImage> img = m_userPushImage->GetImage();
	if (ui->checkBox_tw_img->checkState() > 0) {
		img = m_userPushImage->GetBinaryImgData(ui->threshold_progress_horizontalSlider->value());
	}
	scaledimg = img->scaled(ui->preview_label_temp->width(), ui->preview_label_temp->height(), Qt::KeepAspectRatio);
	ui->preview_label_temp->setPixmap(QPixmap::fromImage(scaledimg));
}