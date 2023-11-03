//
// Created by 19254 on 2023/9/27.
//

#ifndef HW_TOOLS_CONFIG_HPP
#define HW_TOOLS_CONFIG_HPP

#include <string>
#include "nlohmann/json.hpp"

namespace Lib{

class config {
public:

	struct AppConfigModel {

		struct CtrlModel {
			// 扩展路径(历史)
			std::string Path;
			// 插件表
			struct Plugins {
				// 插件启用状态
				bool Enable = false;
				// 插件名称
				std::string Name;
			};
			std::map<std::string, Plugins> Plugins;
		} Ctrl;

		void Parse(const nlohmann::json& json) {
			if (json.contains("ctrl")) {
				auto& ctrl = json["ctrl"];
				if (ctrl.contains("path")) {
					Ctrl.Path = ctrl["path"].get<std::string>();
				}
				if (ctrl.contains("plugins")) {
					auto& plugins = ctrl["plugins"];
					for (auto& iter: plugins.items()) {
						auto val = iter.value();
						auto& name = iter.key();
						Ctrl.Plugins[name] = {};
						Ctrl.Plugins[name].Enable = val["enable"].get<bool>();
						Ctrl.Plugins[name].Name = name;
					}
				}
			}
		}
		std::string dump() {
			nlohmann::json json;
			json["ctrl"]["path"] = Ctrl.Path;
			for (auto& iter: Ctrl.Plugins) {
				json["ctrl"]["plugins"][iter.first]["enable"] = iter.second.Enable;
			}
			return json.dump(4);
		}
	};

public:
    static config* getInstance();
    ~config();
    void saveConfig();

	AppConfigModel& getConfig();


private:
    static config* m_instance;
    config();
	AppConfigModel m_config;
};
}

#define GetConfigInstance() Lib::config::getInstance()
#define GetConfigData() Lib::config::getInstance()->getConfig()



#endif //HW_TOOLS_CONFIG_HPP
