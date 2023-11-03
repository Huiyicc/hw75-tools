//
// Created by 19254 on 2023/10/30.
//

#include "Plugin.hpp"
#include <iostream>
#include <QCoreApplication>
#include "QDir"
#include "nlohmann/json.hpp"

typedef char *(*PluginRegisterFunc)();

typedef bool (*PluginUnRegisterFunc)();

typedef char *(*PluginConfigUIFunc)();

typedef int (*PluginTestFunc)();

typedef bool (*PluginInitFunc)(const char *);

typedef bool (*PluginSaveConfigFunc)(const char *);

typedef bool (*PluginTimedEventFunc)();

typedef bool (*PluginSubmitFunc)(const char *);

typedef char *(*PluginGetLastErrorFunc)();

namespace Lib::Plugin {

#ifdef _WIN32
#define PLUGIN_ADDR ".win"
#endif
#ifdef __linux__
#define PLUGIN_ADDR ".linux"
#endif
#ifdef __APPLE__
#define PLUGIN_ADDR ".mac"
#endif

std::map<QString, std::map<QString, PluginInfo>> g_plugins;
extern std::map<std::string, void *> g_mapPluginFunc;

void PluginsInitAll(const QString &type) {
	// 获取运行目录
	auto appPath = QCoreApplication::applicationDirPath();
	// 判断plugins目录是否存在
	QDir dir_f(appPath + "/plugins");
	if (!dir_f.exists()) {
		// 创建plugins目录
		if (!dir_f.mkdir(appPath + "/plugins")) {
			throw std::runtime_error("创建plugins目录失败");
		}
	}
	QDir dir_t(appPath + "/plugins/" + type + "/");
	if (!dir_t.exists()) {
		// 创建plugins目录
		if (!dir_t.mkdir(appPath + "/plugins/" + type + "/")) {
			throw std::runtime_error("创建plugins目录失败");
		}
	}
	// 加载所有插件
	// 枚举所有文件夹
	auto dirList = dir_t.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto &dirName: dirList) {
		// 加载插件
		auto pluginPath = "./plugins/" + type + "/" + dirName + "/";
		// PluginInfo  info;
		auto info = PluginLoad(pluginPath + dirName + PLUGIN_ADDR);
		info.RawName = dirName;
		info.PluginPath = pluginPath;
		g_plugins[type][dirName] = info;
	}

}


PluginInfo PluginLoad(const QString &path) {
	// 加载插件
	PLUGINHANDLE handle = LoadPlugin(path.toStdWString().c_str());
	if (handle == nullptr) {
		throw std::runtime_error("加载插件失败");
	}
	// 获取插件信息
	auto pluginRegister = (PluginRegisterFunc) GetPluginFunc(handle, "PluginRegister");
	if (pluginRegister == nullptr) {
		throw std::runtime_error("获取插件函数失败");
	}
	auto pluginInfo = pluginRegister();
	// 解析插件信息
	PluginInfo info;
	auto json = nlohmann::json::parse(pluginInfo);

	free(pluginInfo);
	info.Handle = handle;
	info.Name = json["name"].get<std::string>().c_str();
	info.Version = json["version"].get<std::string>().c_str();
	info.Author = json["author"].get<std::string>().c_str();
	info.Description = json["description"].get<std::string>().c_str();

	// 卸载插件
#ifndef _DEBUG
	// 不知道什么原因,加了这句clion会报错
	// UnloadPlugin(handle);
#endif
	return info;
}

std::map<QString, PluginInfo> &GetPlugins(const QString &type) {
	return g_plugins[type];
}

PluginInfo &GetPlugin(const QString &type, const QString &name) {
	return g_plugins[type][name];
}

std::string CallPluginConfigUI(const QString &type, const QString &name) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
//	if (!plugin.Enable) {
//		return "";
//	}
	PluginConfigUIFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginConfigUI") == plugin.CallFuncsTable.end()) {
		auto pluginConfigUI = (PluginConfigUIFunc) GetPluginFunc(plugin.Handle, "PluginConfigUI");
		if (pluginConfigUI == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginConfigUI;
	} else {
		callFunc = (PluginConfigUIFunc) plugin.CallFuncsTable["PluginConfigUI"];
	}
	auto pluginConfigUIResult = callFunc();
	std::string ret = pluginConfigUIResult;
	free(pluginConfigUIResult);
	return ret;
}

int CallPluginTest(const QString &type, const QString &name) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return 0;
	}
	PluginTestFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginTest") == plugin.CallFuncsTable.end()) {
		auto pluginTest = (PluginTestFunc) GetPluginFunc(plugin.Handle, "PluginTest");
		if (pluginTest == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginTest;
	} else {
		callFunc = (PluginTestFunc) plugin.CallFuncsTable["PluginTest"];
	}
	if (callFunc != nullptr) {
		return callFunc();
	}
	return 0;
}

int CallPluginInit(const QString &type, const QString &name, PluginInfo &config) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return 0;
	}
	PluginInitFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginInit") == plugin.CallFuncsTable.end()) {
		auto pluginInit = (PluginInitFunc) GetPluginFunc(plugin.Handle, "PluginInit");
		if (pluginInit == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginInit;
	} else {
		callFunc = (PluginInitFunc) plugin.CallFuncsTable["PluginInit"];
	}
	if (callFunc != nullptr) {
		return callFunc(config.toJsonString(g_mapPluginFunc).c_str());
	}
	return 0;
}

bool CallPluginSaveConfig(const QString &type, const QString &name, const QString &config) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return false;
	}
	PluginSaveConfigFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginSaveConfig") == plugin.CallFuncsTable.end()) {
		auto pluginSaveConfig = (PluginSaveConfigFunc) GetPluginFunc(plugin.Handle, "PluginSaveConfig");
		if (pluginSaveConfig == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginSaveConfig;
	} else {
		callFunc = (PluginSaveConfigFunc) plugin.CallFuncsTable["PluginSaveConfig"];
	}
	if (callFunc != nullptr) {
		return callFunc(config.toStdString().c_str());
	}
	return false;
}

bool CallPluginTimedEvent(const QString &type, const QString &name) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return false;
	}
	PluginTimedEventFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginTimedEvent") == plugin.CallFuncsTable.end()) {
		auto pluginTimedEvent = (PluginTimedEventFunc) GetPluginFunc(plugin.Handle, "PluginTimedEvent");
		if (pluginTimedEvent == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginTimedEvent;
	} else {
		callFunc = (PluginTimedEventFunc) plugin.CallFuncsTable["PluginTimedEvent"];
	}
	if (callFunc != nullptr) {
		return callFunc();
	}
	return false;
}

bool CallPluginSubmit(const QString &type, const QString &name, const QString &data) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return false;
	}
	PluginSubmitFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginSubmit") == plugin.CallFuncsTable.end()) {
		auto pluginTimedEvent = (PluginSubmitFunc) GetPluginFunc(plugin.Handle, "PluginSubmit");
		if (pluginTimedEvent == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = pluginTimedEvent;
	} else {
		callFunc = (PluginSubmitFunc) plugin.CallFuncsTable["PluginSubmit"];
	}
	if (callFunc != nullptr) {
		return callFunc(data.toStdString().c_str());
	}
	return false;
}

std::string CallPluginGetLastError(const QString &type, const QString &name) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return "";
	}
	PluginGetLastErrorFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginGetLastError") == plugin.CallFuncsTable.end()) {
		auto ptr = (PluginGetLastErrorFunc) GetPluginFunc(plugin.Handle, "PluginGetLastError");
		if (ptr == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = ptr;
	} else {
		callFunc = (PluginGetLastErrorFunc) plugin.CallFuncsTable["PluginGetLastError"];
	}
	if (callFunc != nullptr) {
		auto r = callFunc();
		std::string ret = r;
		free(r);
		return ret;
	}
	return "";
}

bool CallPluginUnRegister(const QString &type, const QString &name) {
	auto plugin = g_plugins[type][name];
	if (plugin.Handle == nullptr) {
		throw std::runtime_error("插件未加载");
	}
	if (!plugin.Enable) {
		return "";
	}
	PluginUnRegisterFunc callFunc = nullptr;
	if (plugin.CallFuncsTable.find("PluginUnRegister") == plugin.CallFuncsTable.end()) {
		auto ptr = (PluginUnRegisterFunc) GetPluginFunc(plugin.Handle, "PluginUnRegister");
		if (ptr == nullptr) {
			throw std::runtime_error("获取插件函数失败");
		}
		callFunc = ptr;
	} else {
		callFunc = (PluginUnRegisterFunc) plugin.CallFuncsTable["PluginUnRegister"];
	}
	if (callFunc != nullptr) {
		auto r = callFunc();
		plugin.Enable = false;
		return r;
	}
	return false;
}


} // Lib