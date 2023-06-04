//
// Created by HuiYi on 2023/6/4.
//
#include "mainwindow.h"
#include "core/httpserver.hpp"
#include "utils/coding.hpp"
#include "image/Image.hpp"

#define REGISTERHTTPHANDER(path,hander) http.RegisterCallBack(path,std::bind(hander,this,std::placeholders::_1,std::placeholders::_2))

void MainWindow::httphander() {
	auto http = core::httpserver::Instance();
	if (http.IsRun()) {
		return;
	}
	http.CallBackClear();
	// 获取所有设备列表
	REGISTERHTTPHANDER(core::httpserver::API_PATH_GET_DYNAMIC_List, &MainWindow::httphanderGetDynamiclist);
	// 获取当前绑定设备信息
	REGISTERHTTPHANDER(core::httpserver::API_PATH_GET_DYNAMIC, &MainWindow::httphanderGetDynamic);
	// 推送墨水屏
	REGISTERHTTPHANDER(core::httpserver::API_PATH_ELINK_PUSH, &MainWindow::httphanderElinkPush);

}


void MainWindow::httphanderGetDynamiclist(const httplib::Request &req, httplib::Response &resp) {
	std::vector<Lib::HWDevice> HWDevicesList;
	Lib::HWDeviceTools tools;
	tools.GetHWDevicesList(HWDevicesList);
	nJson json;
	json["version"] = tools.GetDynamicVersion(m_devices).toJson();
	json["list"] = nJson::array();
	for (auto &v: HWDevicesList) {
		json["list"].push_back(v.toJson());
	}
	core::httpserver::SuccessResponst(resp, json);
}

void MainWindow::httphanderElinkPush(const httplib::Request &req, httplib::Response &resp) {
	nJson json = nJson::parse(req.body);
	auto imgdata = utils::Base64DecodeToImage(QString::fromStdString(json["img"].get<std::string>()));
	Lib::HWDeviceTools tools;
	Lib::Image img= imgdata;
	tools.SetDynamicScerrn(QString::fromStdString(json["path"].get<std::string>()),*img.BinaryImgDataToBits(json["threshold"].get<int>()));
	core::httpserver::SuccessResponst(resp);
}

void MainWindow::httphanderGetDynamic(const httplib::Request &req, httplib::Response &resp) {
	nJson json = nJson::object();
	Lib::HWDeviceTools tools;
	json["version"] = tools.GetDynamicVersion(m_devices).toJson();
	json["data"] = m_devices.toJson();
	core::httpserver::SuccessResponst(resp, json);
}
