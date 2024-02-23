//
// Created by 19254 on 2024/2/22.
//

#ifndef HW_TOOLS_CONV_HPP
#define HW_TOOLS_CONV_HPP
#include <filesystem>

namespace Lib::Convert {

// 将BIN文件转换为uf2文件
size_t BinToUf2(const std::string &BinFile, const std::string &Uf2File,uint32_t FamilyID =0x57755a57, uint32_t AppstartAddr = 0x8010000) ;
std::string BinToUf2(const std::string &BinData,uint32_t FamilyID =0x57755a57, uint32_t AppstartAddr = 0x8010000) ;

}// namespace Lib::Convert

#endif//HW_TOOLS_CONV_HPP
