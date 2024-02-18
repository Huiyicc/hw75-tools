#include "http/httpsvr.hpp"
#include "plugin/Plugin.hpp"
#include "ui/mainwindow.h"
#include "utils/Log.hpp"
#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSystemSemaphore>
#include <QTranslator>

std::shared_ptr<QApplication> g_app = nullptr;

MainWindow *g_mainWindow = nullptr;

int main(int argc, char *argv[]) {
  // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  g_app = std::make_shared<QApplication>(argc, argv);

  // 防止多开
  QString key = "__HWTOOLS_LOCK_";
  QSystemSemaphore semaphore(key, 1);
  if (!semaphore.acquire()) {
    QMessageBox::critical(nullptr, "错误", "检查程序锁失败，可能已经有一个实例在运行了");
    exit(-1);
  }
//  QSharedMemory sharedMemory(key);
//  if (!sharedMemory.create(1)) {
//    QMessageBox::critical(nullptr, "错误", "检查程序锁失败，可能已经有一个实例在运行了");
//    exit(-1);
//  }

  // initHTTPSvr();

  MainWindow mainWind;
  try {
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
      const QString baseName = "hw_tools_" + QLocale(locale).name();
      if (translator.load(":/i18n/" + baseName)) {
        g_app->installTranslator(&translator);
        break;
      }
    }
    mainWind.setWindowIcon(QIcon(":/res/default/logo.ico"));
    mainWind.show();
  } catch (std::exception &e) {
    PrintError("错误: {}", e.what());
    QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
    exit(-1);
  }
  g_mainWindow = nullptr;
  int code = g_app->exec();
  return code;
}
