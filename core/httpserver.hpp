//
// Created by HuiYi on 2023/6/3.
//

#ifndef HW_TOOLS_HTTPSERVER_HPP
#define HW_TOOLS_HTTPSERVER_HPP

#include "http/httplib.h"
#include <memory>
#include "nlohmann/json.hpp"

namespace core {

	class httpserver {
	public:
		using HTTPHanderFunc = std::function<void(const httplib::Request &, httplib::Response &)>;

		httpserver() = default;

		~httpserver() = default;

		/** 获取实例  */
		static httpserver &Instance();

		int Port(int port = 0);

		bool IsRun();

		void Start();

		void Stop();

		void RegisterCallBack(const std::string &path, HTTPHanderFunc func);

		void CallBackClear();

		static void SuccessResponst(httplib::Response &resp, nlohmann::json &data);

		static void FailResponst(httplib::Response &resp, int code, const std::string &msg);

	public:

		// 刷新墨水屏
		constexpr static const char *API_PATH_GET_DYNAMIC_List = "/api/dynamic/list";
		constexpr static const char *API_PATH_GET_DYNAMIC = "/api/dynamic/get";
		// 刷新墨水屏
		constexpr static const char *API_PATH_ELINK_PUSH = "/api/dynamic/eink";

	private:
		static int m_port;
		int m_run = false;

		static void callHanlderTampte(const httplib::Request &req, httplib::Response &res);
		static void registerPOSTPath(const std::string &path);

		static void registerGETPath(const std::string &path);

		static std::map<std::string, std::vector<HTTPHanderFunc>> m_router_list;

		static httpserver m_instance;
		static std::shared_ptr<httplib::Server> m_svr;

	};

	class HTTPServerException : public std::exception {
	public:
		HTTPServerException(int code, const std::string &message) : m_code(code), m_message(message) {}

		virtual const char *what() const noexcept {
			return m_message.c_str();
		}

		int code() const noexcept {
			return m_code;
		}

	private:
		int m_code;
		std::string m_message;
	};
}

#endif //HW_TOOLS_HTTPSERVER_HPP
