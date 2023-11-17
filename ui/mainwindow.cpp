#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/widget/MsgBox.hpp"
#include <QMouseEvent>
#include <QTabBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
	try {
		ui->setupUi(this);
		// 设置无边框
		this->setWindowFlag(Qt::FramelessWindowHint);

		connect(ui->systitle_button_min, &QPushButton::clicked, this, &MainWindow::sysButtonEventMin);

		initMenu();
		m_sysTrayIcon.setToolTip("tt");
		m_sysTrayIcon.setIcon(QIcon("://res/default/logo.ico"));
		m_sysTrayIcon.show();

		ctrlInit(parent);
        knobInit(parent);
		ctrlEinkInit(parent);
		ctrlOLEDInit(parent);
		ctrlPluginInit(parent);

	}catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		MsgBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
		exit(-1);
	}
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    //QPoint y = event->globalPosition().toPoint();
    QPoint y = event->globalPos();
    QPoint x = y - z;
    this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    QPoint x = this->geometry().topLeft();
    //QPoint y = event->globalPosition().toPoint();
    QPoint y = event->globalPos();
    z = y - x;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}

void MainWindow::sysButtonEventMin(bool checked) {
    hide();
}