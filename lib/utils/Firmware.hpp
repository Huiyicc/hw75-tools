//
// Created by 19254 on 2024/1/14.
//

#ifndef HW_TOOLS_FIRMWARE_HPP
#define HW_TOOLS_FIRMWARE_HPP

#include<string>
#include <sstream>

namespace utils {

namespace firmware {

std::string GetFirmwareTag(const std::string& FirmwarePath, const std::string& BeginTag, const std::string& EndTag);

};

} // Lib

#endif //HW_TOOLS_FIRMWARE_HPP
