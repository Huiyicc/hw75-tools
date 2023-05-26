#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HID/devices.hpp"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

	~MainWindow();

private:
	Ui::MainWindow *ui;

	Lib::HWDevice m_devices;

	// 第一次显示窗口时调用
	void showEvent(QShowEvent *event) override;

	void eventByDynamicComboxUpdateTable(int index);

	// 更新电机扩展标签信息
	void updateDynamicLabel();

};

#endif // MAINWINDOW_H
