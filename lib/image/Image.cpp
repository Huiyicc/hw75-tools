//
// Created by HuiYi on 2023/5/28.
//

#include "Image.hpp"
#include "QBuffer"
#include "utils/Log.hpp"
#include <iostream>

// 将一个8位二进制字符串转换为char
char binaryStringToChar(const std::string &binaryStr) {
    char result = 0;
    for (size_t i = 0; i < 8; i++) {
        result = (result << 1) | (binaryStr[i] - '0');
    }
    return result;
}

namespace Lib {
Image::Image() : Image(128, 296) {}

Image::Image(int weight, int height) {
    m_image = std::make_shared<QImage>(weight, height, QImage::Format_ARGB32);
    // 设置颜色
    QColor color(255, 255, 255);
    QRgb rgbValue = color.rgba();
    m_image->fill(rgbValue);
};

Image::Image(const QString &path) {
    m_image = std::make_shared<QImage>(path);
}

Image::Image(const QImage &img) {
    m_image = std::make_shared<QImage>(img);
};

Image &Image::LoadToQStringByte(const QString &data) {
    QByteArray imageData = data.toUtf8(); // 将QString转换为UTF-8编码的QByteArray
    QImage image = QImage::fromData(imageData);
    PrintDebug("LoadToQStringByte image width: {}",image.size().width());
    return *this;
};

std::shared_ptr<QImage> Image::GetImage() {
    return m_image;
}

std::shared_ptr<QImage> Image::GetBinaryImgData(int threshold, int width, int height) {
    // 检查图像尺寸
    auto sz = m_image->size();
    if (sz.height() != height || sz.width() != width) {
        // 缩放图像
        QImage rimg = m_image->scaled(width, height);
        std::shared_ptr<QImage> newImage = std::make_shared<QImage>(rimg);
        // 更新m_image为新的图像数据
        m_image = newImage;
    }
    // 转换为灰度图像
    std::shared_ptr<QImage> grayImage = std::make_shared<QImage>(m_image->convertToFormat(QImage::Format_Grayscale8));
    // 应用二值化算法
    for (int y = 0; y < grayImage->height(); ++y) {
        for (int x = 0; x < grayImage->width(); ++x) {
            QRgb pixel = grayImage->pixel(x, y);
            int grayValue = qGray(pixel);
            if (grayValue < threshold) {
                grayImage->setPixel(x, y, qRgb(0, 0, 0)); // 黑色像素
            } else {
                grayImage->setPixel(x, y, qRgb(255, 255, 255)); // 白色像素
            }
        }
    }

    return grayImage;
}

int Image::GetMaximumThreshold() {
    // 转换为灰度图像
    QImage grayImage = m_image->convertToFormat(QImage::Format_Grayscale8);
    // 对灰度图像计算直方图，获取每个灰度级别的像素数量。
    QVector<int> histogram(256, 0); // 初始化直方图向量，索引表示灰度级别
    for (int y = 0; y < grayImage.height(); ++y) {
        for (int x = 0; x < grayImage.width(); ++x) {
            QRgb pixel = grayImage.pixel(x, y);
            int grayValue = qGray(pixel);
            histogram[grayValue]++;
        }
    }
    // 根据直方图，找到像素数量最多的灰度级别，即最大阈值。
    int maxThreshold = 0;
    int maxCount = 0;
    for (int i = 0; i < histogram.size(); ++i) {
        if (histogram[i] > maxCount) {
            maxCount = histogram[i];
            maxThreshold = i;
        }
    }
    return maxThreshold;
}

Image::~Image() {
    m_image.reset();
}

std::shared_ptr<QByteArray> Image::ToBits() {
    std::shared_ptr<QByteArray> result = std::make_shared<QByteArray>((m_image->sizeInBytes() / 4 + 7) / 8, 0);
    const uchar *bits = m_image->bits();
    for (int i = 0; i < m_image->sizeInBytes() / 4; i++) {
        (*result)[i / 8] = static_cast<char>((*result)[i / 8] | ((bits[i * 4] ? 1 : 0) << (7 - i % 8)));
    }
    return result;
}

std::shared_ptr<std::vector<unsigned char>>
Image::BinaryImgDataToBits(int threshold, int width, int height, bool mirrored) {
    auto lst = GetBinaryImgData(threshold, width, height);
    QImage img;
    if (mirrored) {
        img = lst->mirrored(true, false);
    } else {
        img = *lst;
    }
    int pixelCount = img.width() * img.height();
    std::shared_ptr<std::vector<unsigned char>> output = std::make_shared<std::vector<unsigned char>>();
    auto count = img.size().width() * img.size().height();
    if (count % 8 != 0) {
        throw std::runtime_error("图像尺寸不正确");
    }
    output->resize((pixelCount + 7) / 8);
    std::fill(output->begin(), output->end(), 0);
    for (int i = 0; i < pixelCount; i++) {
        int x = i % img.width();
        int y = i / img.width();
        QColor color(img.pixelColor(x, y));
        (*output)[i / 8] |= ((color.red() != 0 ? 1 : 0) << (7 - i % 8));
    }

    return output;
}

std::shared_ptr<std::vector<unsigned char>>
Image::BinaryImgDataToOLEDBits(int threshold, bool mirrored, int width, int height) {
    auto rest = BinaryImgDataToBits(threshold, width, height, mirrored);
    // 分组排序有问题
    // |4|3|2|1| -> |1|2|3|4|
    auto data = rest->data();
    for (int i = 0; i < height*width/8; i += 4) {
        auto tmpC = data[i];
        data[i]=data[i+3];
        data[i+3]=tmpC;
        tmpC = data[i+1];
        data[i+1]=data[i+2];
        data[i+2]=tmpC;
    }
    return rest;
}


} // Lib