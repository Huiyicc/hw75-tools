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
    static config* getInstance();
    ~config();
    void saveConfig();
    nlohmann::json& getConfig();

private:
    static config* m_instance;
    config();
    nlohmann::json m_config;
};
}

#define GetConfigInstance() Lib::config::getInstance()
#define GetConfigJson() Lib::config::getInstance()->getConfig()



#endif //HW_TOOLS_CONFIG_HPP
