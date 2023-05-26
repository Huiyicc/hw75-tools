#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::showEvent(QShowEvent *event) {
	connect(ui->comboBox_motor_equipment, QOverload<int>::of(&QComboBox::currentIndexChanged),
	        this, &MainWindow::eventByDynamicComboxUpdateTable);
	try {
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
	// 更新缓存设备信息
	m_devices = ui->comboBox_motor_equipment->itemData(index).value<Lib::HWDevice>();
}

void MainWindow::updateDynamicLabel() {
	Lib::HWDeviceTools tools;
	auto version= tools.GetVersion(m_devices);
	ui->app_version_lable->setText(version.AppVersion);
	ui->zmk_version_lable->setText(version.ZmkVersion);
	ui->zephyr_version_lable->setText(version.ZephyrVersion);

}
