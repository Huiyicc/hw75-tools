//
// Created by 19254 on 2023/9/27.
//

#ifndef HW_TOOLS_CONFIG_HPP
#define HW_TOOLS_CONFIG_HPP

#include <string>
#include "nlohmann/json.hpp"

namespace Lib {

class config {
public:

    struct AppConfigModel {

        struct CtrlModel {
            // 插件表
            struct Plugins {
                // 插件启用状态
                bool Enable = false;
                // 插件名称
                std::string Name;
            };
            // 固件相关配置
            struct Firmware {
                // 更新源
                int UpdateSource=0;
            };
            // 扩展路径(历史保存)
            std::string Path;
            // 保存插件启用状态
            std::map<std::string, Plugins> Plugins;
            // 固件相关配置
            Firmware Firmware;
        } Ctrl;

        void Parse(const nlohmann::json &json) {
          if (json.contains("ctrl")) {
            auto &ctrl = json["ctrl"];
            if (ctrl.contains("path")) {
              Ctrl.Path = ctrl.value<std::string>("path","");
            }
            if (ctrl.contains("plugins")) {
              auto &plugins = ctrl["plugins"];
              for (auto &iter: plugins.items()) {
                auto val = iter.value();
                auto &name = iter.key();
                Ctrl.Plugins[name] = {};
                Ctrl.Plugins[name].Enable = val.value<bool>("enable",false);
                Ctrl.Plugins[name].Name = name;
              }
            }
            if (ctrl.contains("firmware")) {
              Ctrl.Firmware.UpdateSource = ctrl.value<int>("firmware",0);
            }
          }
        }

        std::string dump() {
          nlohmann::json json;
          json["ctrl"]["path"] = Ctrl.Path;
          for (auto &iter: Ctrl.Plugins) {
            json["ctrl"]["plugins"][iter.first]["enable"] = iter.second.Enable;
          }
          json["ctrl"]["firmware"] = Ctrl.Firmware.UpdateSource;
          return json.dump(4);
        }
    };

public:
    static config &getInstance();

    ~config();

    void saveConfig();

    AppConfigModel &getConfig();

    AppConfigModel *operator->(){
      return &m_config;
    };

private:
    static config *m_instance;

    config();

    AppConfigModel m_config;
};
}

#define GetConfigInstance() Lib::config::getInstance()


#endif //HW_TOOLS_CONFIG_HPP
