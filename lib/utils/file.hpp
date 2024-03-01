//
// Created by HuiYi on 2023/6/3.
//

#ifndef HW_TOOLS_FILE_HPP
#define HW_TOOLS_FILE_HPP
#include "string"
#include <QTextStream>

namespace utils::files {
/** FileIfSet 判断文件是否存在
 * @param filePath：文件路径
 * @param create：文件不存在时是否自动创建 */
	bool FileIfSet(const std::string &filePath, bool create = false,const std::string &content="");

	/** WriteFile 向文件写入数据
 * @param filePath：文件路径
 * @param content：欲写入的数据 */
	bool WriteFile(const QString &fileName, const QString &content);
  /** WriteFile 向文件写入数据
 * @param filePath：文件路径
 * @param content：欲写入的数据 */
  bool WriteFile(const std::string &fileName, const char* data,int size);
  /** WriteFile 向文件写入数据
 * @param filePath：文件路径
 * @param content：欲写入的数据 */
  bool WriteFile(const QString &fileName, const char* data,int size);

	/** ReadFile 读入文件数据
 * @param filePath：文件路径 */
  QByteArray ReadFile(const QString &fileName);

/**
 * @brief 从文件路径中提取文件名
 *
 * 该函数从给定的文件路径中提取文件名。
 *
 * @param filePath 文件路径的字符串
 * @param withExtension 是否包含文件扩展名，默认为 true
 * @return 提取的文件名（或文件名和扩展名，取决于 withExtension 参数）
 *
 * @note 如果文件路径为空字符串，则返回空字符串。
 *
 * 示例用法：
 * @code
 * std::string filePath = "路径/到/您的/文件.txt";
 * std::string fileNameWithExtension = FileExtractName(filePath, true);
 * std::string fileNameWithoutExtension = FileExtractName(filePath, false);
 * @endcode
 */
std::string FileExtractName(const std::string &filePath, bool withExtension=true);

/**
 * @brief 从文件路径中提取文件扩展名
 *
 * 该函数从给定的文件路径中提取文件扩展名。
 *
 * @param filePath 文件路径的字符串
 * @return 提取的文件扩展名
 *
 * @note 如果文件路径为空字符串，则返回空字符串。
 *
 * 示例用法：
 * @code
 * std::string filePath = "路径/到/您的/文件.txt";
 * std::string fileExtension = FileExtractExtension(filePath);
 * @endcode
 */
std::string FileExtractExtension(const std::string &filePath);

}

#endif //HW_TOOLS_FILE_HPP
