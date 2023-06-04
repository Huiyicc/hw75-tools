#include <QApplication>
#include <QMessageBox>
#include "ui/mainwindow.h"
#include "core/config.hpp"
#include "core/httpserver.hpp"

void initFunc(){
	try {
		auto conf = core::config::Instance();
		bool ifs = conf.GetValue<bool>(core::config::HTTPAPI_ENABLE, false);
		if (ifs) {
			int port = conf.GetValue<int>(core::config::HTTPAPI_PORT, 17980);
			auto httpsvr = core::httpserver::Instance();
			httpsvr.Port(port);
			httpsvr.Start();
		}
	}catch (std::exception &e){
		std::cout<<e.what()<<std::endl;
		QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
		exit(-1);
	}
}

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	initFunc();
	MainWindow w;
	w.setFixedSize(1186, 666);
	w.show();
	return a.exec();
}
