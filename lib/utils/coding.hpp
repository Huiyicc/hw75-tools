//
// Created by HuiYi on 2023/6/4.
//

#ifndef HW_TOOLS_CODING_HPP
#define HW_TOOLS_CODING_HPP
#include <QCoreApplication>
#include <QByteArray>
#include <QImage>
#include <iomanip>
#include <sstream>

namespace utils{

	/** Base64解码 */
	QString Base64Decode(const QString& base64String);
	QImage Base64DecodeToImage(const QString& base64Data);

  template <typename T>
  std::string VectorToHexStr(const std::vector<T>& data,bool format=false) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (unsigned char c : data) {
      if (format) {
        ss << "0x" << std::setw(2) << static_cast<unsigned int>(c) << ", ";
      } else {
        ss << std::setw(2) << static_cast<unsigned int>(c);
      }
    }
    return ss.str();
  }
}



#endif //HW_TOOLS_CODING_HPP
