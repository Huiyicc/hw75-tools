//
// Created by 19254 on 2023/9/27.
//

#include "config.hpp"
#include "Log.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

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
    auto bstr = buffer.str();
    if (bstr.empty()) {
      bstr = "{}";
    }
    m_config.Parse(nlohmann::json::parse(bstr));
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
    PrintError("Unable to open file for writing");
  }
}

config::AppConfigModel &config::getConfig() {
  return m_config;
}


}// namespace Lib