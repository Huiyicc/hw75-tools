//
// Created by 19254 on 2024/1/14.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Firmware.hpp"

namespace utils::firmware {


std::string GetFirmwareTag(const std::string &FirmwarePath, const std::string &BeginTag, const std::string &EndTag) {
  // 读入文件(文件为二进制)
  std::ifstream file(FirmwarePath, std::ios::binary);
  if (!file.is_open()) {
    return {};
  }
  std::string tag;
  std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  file.close();
  size_t beginPos = fileContent.find(BeginTag);
  if (beginPos == std::string::npos) {
    std::cerr << "BeginTag not found in file." << std::endl;
    return {};
  }

  beginPos += BeginTag.length();

  size_t endPos = fileContent.find(EndTag, beginPos);
  if (endPos == std::string::npos) {
    std::cerr << "EndTag not found in file." << std::endl;
    return {};
  }
  //std::stringstream{};
  return fileContent.substr(beginPos, endPos - beginPos);
}


} // utils::firmware