#include "build_info.h"
#include "http/httpsvr.hpp"
#include "plugin/Plugin.hpp"
#include "ui/mainwindow.h"
#include "utils/Log.hpp"
#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSystemSemaphore>
#include <QTranslator>

QApplication* g_app = nullptr;

// std::shared_ptr<MainWindow> g_mainWindowPtr(nullptr, [](MainWindow* ptr) {});

MainWindow *g_mainWindow = nullptr;
int main(int argc, char *argv[]) {
  // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  g_app = new QApplication(argc, argv);

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
  g_mainWindow = &mainWind;
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
    g_mainWindow->setWindowIcon(QIcon(":/res/default/logo.ico"));
    g_mainWindow->show();
  } catch (std::exception &e) {
    PrintError("错误: {}", e.what());
    QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));
    ;
    exit(-1);
  }
  int code = g_app->exec();
  g_app->exit(0);
  delete g_app;
  return code;
}
