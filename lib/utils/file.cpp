//
// Created by HuiYi on 2023/6/3.
//
#include "file.hpp"
#include <QDir>
#include <QFile>

namespace utils {
	/** FileIfSet 判断文件是否存在
 * @param filePath：文件路径
 * @param create：文件不存在时是否自动创建 */
	bool FileIfSet(const std::string &filePath, bool create,const std::string &content) {
		QFile file(QString::fromStdString(filePath));
		if (!file.exists()) {
			// 文件不存在，创建
			QDir().mkpath(QFileInfo(file).path()); // 创建目录，如果不存在的话
			file.open(QIODevice::WriteOnly); // 创建文件
			file.write(content.c_str());
			file.close();
			return false;
		} else {
			return true;
		}
	}

	QString ReadFile(const QString &fileName) {
		QFile file(fileName);
		if (!file.exists()) {
			// 文件不存在，创建
			QDir().mkpath(QFileInfo(file).path());// 创建目录，如果不存在的话
		}
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			return QString();
		}
		QTextStream in(&file);
		QString contents = in.readAll();
		file.close();
		return contents;
	}

	bool WriteFile(const QString &fileName, const QString &content) {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return false;
		}

		QTextStream out(&file);
		out << content;
		file.close();
		return true;
	}
}
