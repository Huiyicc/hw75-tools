//
// Created by HuiYi on 2023/6/4.
//
#include "mainwindow.h"
#include "core/httpserver.hpp"

void MainWindow::httphander() {
	auto http = core::httpserver::Instance();
	if(http.IsRun()){
		return;
	}
	http.CallBackClear();
	http.RegisterCallBack(core::httpserver::API_PATH_GET_DYNAMIC_List, std::bind(&MainWindow::httphanderGetDynamiclist, this, std::placeholders::_1, std::placeholders::_2));
	http.RegisterCallBack(core::httpserver::API_PATH_GET_DYNAMIC, std::bind(&MainWindow::httphanderGetDynamic, this, std::placeholders::_1, std::placeholders::_2));


}


void MainWindow::httphanderGetDynamiclist(const httplib::Request &req, httplib::Response &resp){
	std::vector<Lib::HWDevice> HWDevicesList;
	Lib::HWDeviceTools tools;
	tools.GetHWDevicesList(HWDevicesList);
	nJson json;
	json["list"]=nJson::array();
	for(auto&v:HWDevicesList){
		json["list"].push_back(v.toJson());
	}
	core::httpserver::SuccessResponst(resp, json);
}

void MainWindow::httphanderGetDynamic(const httplib::Request &req, httplib::Response &resp){
	nJson json = m_devices.toJson();
	core::httpserver::SuccessResponst(resp, json);
}
