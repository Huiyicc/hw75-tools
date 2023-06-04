//
// Created by HuiYi on 2023/6/3.
//

#ifndef HW_TOOLS_FILE_HPP
#define HW_TOOLS_FILE_HPP
#include "string"
#include <QTextStream>

namespace utils {
/** FileIfSet 判断文件是否存在
 * @param filePath：文件路径
 * @param create：文件不存在时是否自动创建 */
	bool FileIfSet(const std::string &filePath, bool create = false,const std::string &content="");

	/** WriteFile 向文件写入数据
 * @param filePath：文件路径
 * @param content：欲写入的数据 */
	bool WriteFile(const QString &fileName, const QString &content);

	/** ReadFile 读入文件数据
 * @param filePath：文件路径 */
	QString ReadFile(const QString &fileName);

}

#endif //HW_TOOLS_FILE_HPP
