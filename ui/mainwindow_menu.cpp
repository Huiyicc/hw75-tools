//
// Created by 19254 on 2023/10/21.
//
#include "./ui_mainwindow.h"
#include "mainwindow.h"

extern std::shared_ptr<QApplication> g_app;

void MainWindow::initMenu() {
  // 添加菜单项
  static QAction action_close("退出");
  //static QAction action2("菜单项2");
  // 将菜单项添加到上下文菜单
  //m_trayMenu.addAction(&action2);

  m_trayMenu.addAction(&action_close);
  // 将上下文菜单与托盘图标关联
  m_sysTrayIcon.setContextMenu(&m_trayMenu);
  // 连接菜单项的槽函数，以处理点击事件
  QObject::connect(&action_close, &QAction::triggered, [&]() {
    //        close();
    //	    exit(0);
    QApplication::quit();
  });

  //    QObject::connect(&action2, &QAction::triggered, [&]() {
  //    });

  // 处理双击托盘图标事件
  QObject::connect(&m_sysTrayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
      // 双击托盘图标时还原窗口
      if (isHidden()) {
        show();
        setWindowState(windowState() & (~Qt::WindowMinimized | Qt::WindowActive));
      }
    }
  });
}