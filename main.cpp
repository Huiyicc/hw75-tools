#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include "ui/mainwindow.h"
#include "http/httpsvr.hpp"
#include "plugin/Plugin.hpp"

std::shared_ptr<QApplication> g_app = nullptr;

MainWindow *g_mainWind = nullptr;

int main(int argc, char *argv[]) {
    g_app = std::make_shared<QApplication>(argc, argv);

    initHTTPSvr();

	MainWindow mainWind;
	g_mainWind = &mainWind;
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
        std::cout << e.what() << std::endl;
        QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
        exit(-1);
    }
    int code = g_app->exec();
    return code;
}
