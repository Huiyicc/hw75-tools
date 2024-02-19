//
// Created by 19254 on 2023/10/30.
//

#ifndef HW_TOOLS_PLUGIN_HPP
#define HW_TOOLS_PLUGIN_HPP

#include <qstring.h>
#include <map>
#include "nlohmann/json.hpp"

#if defined (WIN32) | defined (WIN64)

#include "windows.h"

#else
#include "dlfcn.h"
#endif

namespace Lib::Plugin {


#if defined (WIN32) | defined (WIN64)


// 插件句柄
typedef HINSTANCE PLUGINHANDLE;
// 加载插件
#define LoadPlugin(path) LoadLibraryW(path.toStdWString().c_str())
// 获取插件函数
#define GetPluginFunc(handle, funcName) GetProcAddress(handle, funcName)
// 卸载插件
#define UnloadPlugin(handle) FreeLibrary(handle)

#else
// 插件句柄
typedef void* PLUGINHANDLE;

// 加载插件
#define LoadPlugin(path) dlopen(path.toStdString().c_str(),RTLD_LAZY)
// 获取插件函数
#define GetPluginFunc(handle, funcName) dlsym(handle, funcName)
// 卸载插件
#define UnloadPlugin(handle) dlclose(handle)

#endif

struct PluginInfo {
	// 启用状态
	bool Enable = false;
	// 插件句柄
	PLUGINHANDLE Handle = nullptr;
	// 插件名称
	QString Name;
	// 插件原始名字(文件名,不带后缀)
	QString RawName;
	// 插件版本
	QString Version;
	// 插件作者
	QString Author;
	// 插件描述
	QString Description;
	// 插件路径
	QString PluginPath;

	// 插件映射表
	std::map<std::string, void *> CallFuncsTable;


	std::string toJsonString(std::map<std::string, void *> calls) {
		nlohmann::json json;
		json["enable"] = Enable;
		json["name"] = Name.toStdString();
		json["raw_name"] = RawName.toStdString();
		json["version"] = Version.toStdString();
		json["author"] = Author.toStdString();
		json["description"] = Description.toStdString();
		json["plugin_path"] = PluginPath.toStdString();
		json["calls"] = nlohmann::json::array();
		for (auto &[name, func]: calls) {
			auto tmp = nlohmann::json::object();
			tmp["name"] = name;
			tmp["address"] = (uint64_t) func;
			json["calls"].push_back(tmp);
		}
		return json.dump();
	}
};

enum ButtonPinCallType {
  // 左侧按钮短按事件
  LeftButtonPressed = 1,
  // 右侧按钮短按事件
  RightButtonPressed = 2,
  // 左侧按钮长按事件
  LeftButtonLongPress = 3,
  // 右侧按钮长按事件
  RightButtonLongPress = 4,
  Default = 0,
};

// 初始化插件
void PluginsInitAll(const QString &type);

PluginInfo PluginLoad(const QString &path);

std::map<QString, PluginInfo> &GetPlugins(const QString &type);

PluginInfo &GetPlugin(const QString &type, const QString &name);

std::string CallPluginConfigUI(const QString &type, const QString &name);

bool CallPluginTest(const QString &type, const QString &name, const QString &data);

int CallPluginInit(const QString &type, const QString &name, PluginInfo &config);

bool CallPluginSaveConfig(const QString &type, const QString &name, const QString &config);

bool CallPluginTimedEvent(const QString &type, const QString &name);

bool CallPluginSubmit(const QString &type, const QString &name, const QString &data);

std::string CallPluginGetLastError(const QString &type, const QString &name);

bool CallPluginUnRegister(const QString&type, const QString&name);

void CallPluginButtonPin(const QString &type,ButtonPinCallType pin);
bool CallPluginButtonPin(const QString &type, const QString &name,ButtonPinCallType pin);


} // Lib

#endif //HW_TOOLS_PLUGIN_HPP
