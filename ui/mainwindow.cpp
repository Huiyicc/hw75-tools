#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/widget/MsgBox.hpp"
#include "utils/Log.hpp"
#include "utils/defer.hpp"
#include <QMessageBox>
#include <QMouseEvent>
#include <QTabBar>
#include <filesystem>
#include <fmt/format.h>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  try {
    g_mainWindow = this;
    ui->setupUi(this);
    // 设置无边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    connect(ui->systitle_button_min, &QPushButton::clicked, this, &MainWindow::sysButtonEventMin);
    connect(ui->ctrl_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::ctrlEventTabChanged);

    initMenu();
    m_sysTrayIcon.setToolTip("tt");
    m_sysTrayIcon.setIcon(QIcon("://res/default/logo.ico"));
    m_sysTrayIcon.show();

    // 检查res文件夹
    if (!fs::exists("./res")) {
      // 不存在创建
      try {
        fs::create_directories("./res");
      } catch (const std::exception &e) {
        throw std::runtime_error(fmt::format("创建res资源文件夹失败!\n{}", e.what()));
      }
    }

    ctrlInit(parent);
    ctrlHomeInit(parent);
    ctrlRgbInit(parent);
    ctrlSettingInit(parent);
    knobInit(parent);
    ctrlEinkInit(parent);
    ctrlOLEDInit(parent);
    ctrlPluginInit(parent);
    knobChatsInit(parent);
  } catch (std::exception &e) {
    PrintError("错误: {}", e.what());
    QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));
    exit(-1);
  }
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  QWidget::mouseMoveEvent(event);
  QPoint y = event->globalPosition().toPoint();
  // QPoint y = event->globalPos();
  QPoint x = y - z;
  this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  QWidget::mousePressEvent(event);
  QPoint x = this->geometry().topLeft();
  QPoint y = event->globalPosition().toPoint();
  //QPoint y = event->globalPos();
  z = y - x;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
}

void MainWindow::sysButtonEventMin(bool checked) {
  hide();
}

bool MainWindow::checkCtrlConnect() {
  if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) {
    // TODO: 未连接设备
    return false;
  }
  auto dev = hid_open_path(m_modelConnectStatus[HW_MODEL_NAME_CTRL].Path.toStdString().c_str());
  if (dev == nullptr) {
    return false;
  }
  DEFER(hid_close(dev));
  return true;
}

Lib::HWDevice MainWindow::getCtrlConnectDev() {
  return m_modelConnectStatus[HW_MODEL_NAME_CTRL];
}

MainWindow::TabChanged ctrlLastTabChanged = MainWindow::TabChanged(0);

void MainWindow::ctrlEventTabChanged(int index) {
  if (ctrlLastTabChanged == index) {
    return;
  }
  ctrlLastTabChanged = MainWindow::TabChanged(index);
  if (index == TabChanged::SETTING) {
    // 扩展系统设置
    ctrlSettingTabChanged();
  } else if (index == TabChanged::RGB) {
    // 扩展RGB
    ctrlRGBTabChanged();
  } else {
    knobChatsEventShowTable(index == TabChanged::KNOB_DEBUG);
  }
}
