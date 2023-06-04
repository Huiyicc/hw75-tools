//
// Created by HuiYi on 2023/6/4.
//

#include "coding.hpp"

namespace utils{
	QString Base64Decode(const QString& base64String) {
		QByteArray base64Data = QByteArray::fromBase64(base64String.toUtf8());
		QString decodedString = QString::fromUtf8(base64Data);
		return decodedString;
	}

	QImage Base64DecodeToImage(const QString& base64Data) {
		QByteArray imageData = QByteArray::fromBase64(base64Data.toUtf8());
		QImage image = QImage::fromData(imageData);
		return image;
	}
}