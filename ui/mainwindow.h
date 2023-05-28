#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "HID/devices.hpp"
#include "image/Image.hpp"
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

	std::shared_ptr<Lib::Image> m_userPushImage = std::make_shared<Lib::Image>();

	void eventRegister();

	// 第一次显示窗口时调用
	void showEvent(QShowEvent *event) override;

	void eventByDynamicComboxUpdateTable(int index);

	void eventByElinkSwitchImage(bool checked = false);

	void eventByPushImage(bool checked = false);

	void eventByValueChanged(int value);

	// 更新电机扩展标签信息
	void updateDynamicLabel();

	// 更新用户自行选择图像的预览
	void updateUserPreviewImage();

};

#endif // MAINWINDOW_H
