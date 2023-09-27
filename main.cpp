#include "ui/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    try {
        QTranslator translator;
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for (const QString &locale: uiLanguages) {
            const QString baseName = "hw_tools_" + QLocale(locale).name();
            if (translator.load(":/i18n/" + baseName)) {
                a.installTranslator(&translator);
                break;
            }
        }
        w.show();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
        exit(-1);
    }

    return a.exec();
}
