//
// Created by HuiYi on 2023/6/4.
//

#ifndef HW_TOOLS_CODING_HPP
#define HW_TOOLS_CODING_HPP
#include <QCoreApplication>
#include <QByteArray>
#include <QImage>

namespace utils{

	/** Base64解码 */
	QString Base64Decode(const QString& base64String);
	QImage Base64DecodeToImage(const QString& base64Data);
}



#endif //HW_TOOLS_CODING_HPP
