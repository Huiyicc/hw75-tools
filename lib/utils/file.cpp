//
// Created by HuiYi on 2023/6/3.
//
#include "file.hpp"
#include "defer.hpp"
#include <QDir>
#include <QFile>

namespace utils::files {
/** FileIfSet 判断文件是否存在
* @param filePath：文件路径
* @param create：文件不存在时是否自动创建 */
bool FileIfSet(const std::string &filePath, bool create, const std::string &content) {
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

QByteArray ReadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.exists()) {
    return {};
    // 文件不存在，创建
    // QDir().mkpath(QFileInfo(file).path());// 创建目录，如果不存在的话
  }
  if (!file.open(QIODevice::ReadOnly)) {
    return {};
  }
  DEFER(file.close());
  auto binaryData = file.readAll();
  return binaryData;
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
bool WriteFile(const QString &fileName, const char* data,int size) {
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly)) {
    return false;
  }
  file.write(data,size);
  file.close();
  return true;
}
bool WriteFile(const std::string &fileName, const char* data,int size) {
  return WriteFile(QString::fromStdString(fileName),data,size);
}

// 给定一个路径返回文件名
std::string FileExtractName(const std::string &filePath, bool withExtension) {
  if (filePath.empty()) {
    return "";
  }
  std::filesystem::path pathObj(filePath);

  if (withExtension) {
    return pathObj.filename().string();
  } else {
    return pathObj.stem().string();
  }
}

// 给定一个路径返回文件后缀
std::string FileExtractExtension(const std::string &filePath) {
  if (filePath.empty()) {
    return "";
  }
  std::filesystem::path pathObj(filePath);
  return pathObj.extension().string();
}

}
