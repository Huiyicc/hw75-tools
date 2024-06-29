#include "build_info.h"
#include "http/httpsvr.hpp"
#include "plugin/Plugin.hpp"
#include <spdlog/sinks/daily_file_sink.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "ui/mainwindow.h"
#include "utils/Log.hpp"
#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QSystemSemaphore>
#include <QTranslator>

QApplication *g_app = nullptr;

// std::shared_ptr<MainWindow> g_mainWindowPtr(nullptr, [](MainWindow* ptr) {});

#ifdef _DEBUG
const auto log_level = spdlog::level::debug;
#else
const auto log_level = spdlog::level::info;
#endif

void logInit() {
  // 创建控制台sink
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level(log_level);// 设置控制台输出级别

  // 创建文件sink
  auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/log", 0, 0);
  daily_sink->set_level(log_level); // 设置文件输出级别

  // 创建logger并将两个sink添加到logger中
  auto logger = std::make_shared<spdlog::logger>("HWTOOLS", spdlog::sinks_init_list{console_sink, daily_sink});
  spdlog::register_logger(logger);
  spdlog::set_default_logger(logger);
  // 设置logger的全局日志级别
  spdlog::set_level(log_level);
}

MainWindow *g_mainWindow = nullptr;
int main(int argc, char *argv[]) {
  int code = 0;
  // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  logInit();
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
    // 初始化上下文
    auto r = hid_init();
    if (r!=0) {
      throw std::runtime_error(QString::fromStdWString(hid_error(nullptr)).toStdString());
    }
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
      const QString baseName = "hw_tools_" + QLocale(locale).name();
      if (translator.load(":/i18n/" + baseName)) {
        QApplication::installTranslator(&translator);
        break;
      }
    }
    g_mainWindow->setWindowIcon(QIcon(":/res/default/logo.ico"));
    g_mainWindow->show();
    code = QApplication::exec();
  } catch (std::exception &e) {
    PrintError("错误: {}", e.what());
    QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));
    exit(-1);
  }
  QApplication::exit(0);
  delete g_app;
  hid_exit();
  return code;
}
