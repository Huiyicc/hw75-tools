//
// Created by HuiYi on 2023/5/28.
//

#ifndef HW_TOOLS_IMAGE_HPP
#define HW_TOOLS_IMAGE_HPP

#include <memory>
#include <QImage>
#include <QColor>

namespace Lib {

/** 图像处理类,非通用扩展,仅用于特化处理瀚文墨水屏图像 */
class Image {
public:
    Image();

    Image(int weight, int height);

    Image(const QString &path);

    Image(const QImage &img);

    ~Image();

    std::shared_ptr<QImage> GetImage();

    /** 用于计算图像用于二值化时最大的可选阈值 */
    int GetMaximumThreshold();

    Image &LoadToQStringByte(const QString &data);

    std::shared_ptr<QImage> GetBinaryImgData(int threshold = 50, int width = 128, int height = 296);

    std::shared_ptr<QByteArray> ToBits();

    std::shared_ptr<std::vector<unsigned char>> BinaryImgDataToBits(int threshold = 50,int width = 128,int height = 296,bool mirrored = true);

    std::shared_ptr<std::vector<unsigned char>> BinaryImgDataToOLEDBits(int threshold = 50,bool mirrored = true,int width = 32,int height = 128);

private:
    std::shared_ptr<QImage> m_image = nullptr;

};

} // Lib

#endif //HW_TOOLS_IMAGE_HPP
