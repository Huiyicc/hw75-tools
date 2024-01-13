//
// Created by 19254 on 2023/9/27.
//

#include "config.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


namespace Lib {
config *config::m_instance = nullptr;

config &config::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new config();
    }
    return *m_instance;
}


config::config() {
    //读取config.json
    std::ifstream file("./config.json");
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
		m_config.Parse(nlohmann::json::parse(buffer.str()));
        file.close();
    } else {
        // m_config = nlohmann::json::object();
    }

}

config::~config() {
    saveConfig();
}

void config::saveConfig() {
    std::ofstream outFile("./config.json");
    if (outFile.is_open()) {
        outFile << m_config.dump();
        outFile.close();
    } else {
        // std::cerr << "Unable to open file for writing" << std::endl;
    }
}

config::AppConfigModel &config::getConfig() {
    return m_config;
}


}