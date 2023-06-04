//
// Created by HuiYi on 2023/6/3.
//

#include <QMessageBox>
#include <QThread>
#include "httpserver.hpp"

namespace core {

	std::thread g_http_thread;

	std::map<std::string, std::vector<httpserver::HTTPHanderFunc>> httpserver::m_router_list;
	httpserver httpserver::m_instance;
	std::shared_ptr<httplib::Server> httpserver::m_svr;
	int httpserver::m_port=17980;

	void httpserver::CallBackClear() {
		m_router_list.clear();
	};

	bool httpserver::IsRun() { return m_run; };

	httpserver &httpserver::Instance() {
		if (m_svr == nullptr) {
			m_svr = std::make_shared<httplib::Server>();
			m_svr->Get("/test", [](const httplib::Request &req, httplib::Response &res) {
				res.set_content("Hello World!", "text/plain");
			});
			m_svr->Get(API_PATH_GET_DYNAMIC_List, callHanlderTampte);
			m_svr->Get(API_PATH_GET_DYNAMIC, callHanlderTampte);
			m_svr->Post(API_PATH_ELINK_PUSH, callHanlderTampte);
		}
		return m_instance;
	};

	int httpserver::Port(int port) {
		if (port != 0) {
			m_port = port;
		}
		return m_port;
	}

	void httpserver::SuccessResponst(httplib::Response &resp,const nlohmann::json &data) {
		nlohmann::json respData;
		respData["code"]=0;
		respData["msg"]="success";
		respData["data"]=data;
		resp.set_content(respData.dump(), "application/json");
	};

	void httpserver::FailResponst(httplib::Response &resp,int code ,const std::string &msg) {
		nlohmann::json respData;
		respData["code"]=code;
		respData["msg"]=msg;
		resp.set_content(respData.dump(), "application/json");
	};

	void httpserver::Start() {
		if (!m_run) {
			m_port;
			g_http_thread = std::thread([&]() {
				try {
					std::cout<<"httpserver::Start("<<m_port<<")"<<std::endl;
					m_run = true;
					m_svr->listen("127.0.0.1", m_port);
				} catch (std::exception &e) {
					m_run = false;
					QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
				}
			});
		}
	}

	void httpserver::Stop() {
		if (m_run) {
			try {
				m_svr->stop();
				m_run = false;
				for (int i = 0; i < 3; ++i) {
					if (g_http_thread.joinable()) {
						g_http_thread.join();
						break;
					}
					QThread::sleep(1);
				}
			} catch (std::exception &e) {
				QMessageBox::critical(nullptr, "错误", QString::fromStdString(e.what()));;
			}
		}
	}

	void httpserver::callHanlderTampte(const httplib::Request &req, httplib::Response &res) {
			auto iter = m_router_list.find(req.path);
			if (iter != m_router_list.end()) {
				try{
					for (auto &func: iter->second) {
						if (func) {
							func(req, res);
						}
					}
				} catch (const HTTPServerException&err) {
					FailResponst(res,err.code(),err.what());
				} catch (const std::exception&err) {
					FailResponst(res,500,err.what());
				}
			}
		}


	void httpserver::RegisterCallBack(const std::string &path, HTTPHanderFunc func) {
		m_router_list[path].push_back(std::move(func));
	}

}

