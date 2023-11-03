//
// Created by HuiYi on 2023/5/22.
//
#include "devices.hpp"
#include <sstream>
#include <thread>
#include "utils/math.hpp"
#include "QThread"
#include <mutex>

std::mutex send_mutex;

void SplitData(const std::vector<unsigned char> &sourceData, std::vector<std::vector<unsigned char>> &messageDatas,
               size_t chunkSize) {
    size_t dataSize = sourceData.size();
    size_t currentIndex = 0;

    while (currentIndex < dataSize) {
        size_t remainingSize = dataSize - currentIndex;
        size_t chunkSizeToCopy = std::min(chunkSize, remainingSize);

        std::vector<unsigned char> chunk(chunkSizeToCopy);
        for (size_t i = 0; i < chunkSizeToCopy; ++i) {
            chunk[i] = static_cast<unsigned char>(sourceData[currentIndex + i]);
        }

        messageDatas.push_back(chunk);
        currentIndex += chunkSizeToCopy;
    }
}

std::string padStringWithZeros(const std::string &str, int desired_length) {
    std::string padded_str = str;

    if (padded_str.length() < desired_length) {
        padded_str.append(desired_length - padded_str.length(), '0');
    }

    return padded_str;
}

namespace Lib {

HWDeviceTools::HWDeviceTools() {
    // 初始化上下文
    auto r = hid_init();
    if (r) {
        throw DeviceException(hid_error(nullptr));
    }
}

HWDeviceTools::~HWDeviceTools() {
    hid_exit();
}

size_t HWDeviceTools::GetHWDevicesList(std::vector<HWDevice> &HWDevicesList) {
    send_mutex.lock();
    HWDevicesList.clear();
    // 获取连接的HID设备列表
    auto devs = hid_enumerate(0x0, 0x0);
    auto cur_dev = devs;

    // 开始遍历设备列表
    while (cur_dev) {
        // 判断设备是否为USB HID设备

        if (cur_dev->product_id == HWPID && cur_dev->vendor_id == HWVID // 判断是否为瀚文设备
            && cur_dev->usage_page == USB_USAGE_PAGE
                ) {

            // 打开设备
            auto handle = hid_open_path(cur_dev->path);
            wchar_t wstr[256];
            HWDevice temp;
            // 获取产品名称
            hid_get_product_string(handle, wstr, sizeof(wstr) / sizeof(wchar_t));
            // 填充结构体
            temp.Path = cur_dev->path;
            temp.ProductId = cur_dev->product_id;
            temp.ProductName = QString::fromStdWString(wstr);
            temp.VendorId = cur_dev->vendor_id;
            temp.InterfaceNumber = cur_dev->interface_number;
            temp.Usage = cur_dev->usage;
            temp.UsagePage = cur_dev->usage_page;
            temp.SerialNumber = QString::fromStdWString(cur_dev->serial_number);

            HWDevicesList.push_back(temp);
            hid_close(handle);
        }
        // 指向下一个设备
        cur_dev = cur_dev->next;
    }
    // 释放设备列表
    hid_free_enumeration(devs);
    send_mutex.unlock();
    return HWDevicesList.size();
};

HWDeviceDynamicVersion HWDeviceTools::GetDynamicVersion(HWDevice &devices) {
    HWDeviceDynamicVersion result;
    auto device = hid_open_path(devices.Path.toStdString().c_str());
    if (!device) {
        throw DeviceException(hid_error(nullptr));
    }
    hid::msg::PcMessage message;
    message.set_id(hid::msg::MessageId::VERSION);
    hid::msg::Nil *nil = message.mutable_nil();

    // 发送消息
    int result_code = sendMessage(device, message);

    // 读取USB设备返回的字节流
    std::shared_ptr<uint8_t> data(new uint8_t[result_code]);
    // 处理返回的字节流
    hid::msg::CtrlMessage ctrlMessage;

    readMessage(device, ctrlMessage, result_code);
    // 判断返回的消息是否为版本消息
    if (!ctrlMessage.has_version()) {
        throw DeviceException(L"message has no version.");
    }
    result.GitVersion = QString::fromStdString(ctrlMessage.version().gitver());
    if (result.GitVersion == "") {
        result.GitVersion = "Alpha";
    }
    result.GitHash = QString::fromStdString(ctrlMessage.version().githash());
    result.GitBranch = QString::fromStdString(ctrlMessage.version().gitbranch());
    // 关闭设备
    hid_close(device);
    return result;

};

void HWDeviceTools::SetDynamicEinkScerrn(HWDevice &devices, std::vector<unsigned char> &imageArrar) {
    SetDynamicEinkScerrn(devices.Path, imageArrar);
};

void HWDeviceTools::SetDynamicEinkScerrn(const QString &devicesPath, std::vector<unsigned char> &imageArrar) {
    SetDynamicScerrn(2, devicesPath, imageArrar);
};

void HWDeviceTools::SetDynamicOLEDScerrn(HWDevice &devices, std::vector<unsigned char> &imageArrar) {
    SetDynamicOLEDScerrn(devices.Path, imageArrar);
}

void HWDeviceTools::SetDynamicOLEDScerrn(const QString &devicesPath, std::vector<unsigned char> &imageArrar) {
    SetDynamicScerrn(3, devicesPath, imageArrar);
}

void HWDeviceTools::SetDynamicScerrn(int id,const QString &devicesPath, std::vector<unsigned char> &imageArrar) {
    send_mutex.lock();
    HWDeviceDynamicVersion result;
    auto device = hid_open_path(devicesPath.toStdString().c_str());
    if (!device) {
        send_mutex.unlock();
        throw DeviceException(hid_error(nullptr));
    }
    auto aaa = imageArrar.data();
    std::vector<std::vector<unsigned char>> messageDatas;
    if (imageArrar.size() > HID_PAYLOAD_SIZE) {
    // 切分消息
        size_t chunkSize = 61;
        size_t dataSize = imageArrar.size();
        size_t currentIndex = 0;
        while (currentIndex < dataSize) {
            size_t remainingSize = dataSize - currentIndex;
            size_t chunkSizeToCopy = std::min(chunkSize, remainingSize);
            std::vector<unsigned char> chunk(HID_REPORT_COUNT, 0);
            std::copy(imageArrar.begin() + currentIndex, imageArrar.begin() + currentIndex + chunkSizeToCopy,
                      chunk.begin() + 3);
            auto scount = HID_REPORT_COUNT - chunk.size();
            if (scount > 0) {
                chunk.insert(chunk.end(), scount, 0);
            }
            chunk[0] = 0x4;
            chunk[1] = ((currentIndex + chunkSize) < dataSize) ? id : 0;
            chunk[2] = chunkSizeToCopy + 1;
            messageDatas.push_back(chunk);
            currentIndex += chunkSizeToCopy;
        }
    } else {
        send_mutex.unlock();
        throw DeviceException(L"image error");
    }

    for (int i = 0; i < messageDatas.size(); i++) {
        auto &msg_data = messageDatas[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        int result_code = hid_write(device, reinterpret_cast<const unsigned char *>(msg_data.data()),
                                    msg_data.size());
        if (result_code < 0) {
            auto err = hid_error(device);
            send_mutex.unlock();
            // 处理发送失败的情况
            throw DeviceException(err);
        }
    }

    // 读取USB设备返回的字节流
    std::shared_ptr<uint8_t> data(new uint8_t[HID_REPORT_COUNT + 1]);
    memset(data.get(), 0, HID_REPORT_COUNT + 1);
    auto messageSize = hid_read(device, data.get(), HID_REPORT_COUNT + 1);
    if (messageSize < 1) {
        auto err = hid_error(device);
        // 关闭设备
        hid_close(device);
        send_mutex.unlock();
        // 处理发送失败的情况
        throw DeviceException(err);
    }
    if (data.get()[0] != 0x4 || data.get()[1] != 0x2 || data.get()[2] != 0x2 || data.get()[2] != 0x2) {
        // 关闭设备
        hid_close(device);
        send_mutex.unlock();
        throw DeviceException(L"推送失败");
    }
    // 关闭设备
    hid_close(device);
    send_mutex.unlock();
}

int HWDeviceTools::readMessage(hid_device_ *dev, hid::msg::CtrlMessage &message, int messageSize) {
    // 读取USB设备返回的字节流
    std::shared_ptr<uint8_t> data(new uint8_t[messageSize]);

    messageSize = hid_read(dev, data.get(), messageSize);
    if (messageSize < 0) {
        auto err = hid_error(dev);
        // 处理发送失败的情况
        throw DeviceException(err);
    }
    auto dataPtr = data.get();
    //第一个字节为报告id,第二个字节为完整性,第三个字节为数据长度
    if (dataPtr[0] != 0x4) {
        return 0;
    }
    if (dataPtr[1] != 0x0) {
        // TODO: 待实现
        return 0;
    }
    int lents = dataPtr[2];
    if (lents > HID_REPORT_COUNT) {
        throw DeviceException(L"data lens is to long.");
    }
    // 根据长度写入流
    google::protobuf::io::ArrayInputStream arrayInput((void *) ((long long) (&data.get()[2])), lents);
    // 解码字节流
    readDelimitedD2P(&arrayInput, &message);
    return messageSize;
};

int HWDeviceTools::sendMessage(hid_device_ *dev, hid::msg::PcMessage &message) {
    send_mutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    int result = 0;
    //序列化消息
    std::string message_str;
    google::protobuf::io::StringOutputStream stringOutput(&message_str);
    std::string message_size;
    google::protobuf::io::StringOutputStream sizeOutput(&message_size);
    // pc消息转为data
    writeDelimitedP2D(message, &stringOutput, &sizeOutput);
    //拆分消息
    auto msg_size = message_str.size();
    auto add_size = message_size.size();
    std::vector<std::vector<unsigned char>> messageDatas;
    if (msg_size > HID_PAYLOAD_SIZE) {
        // 切分消息
        size_t chunkSize = 61;
        size_t dataSize = message_str.size() - add_size;
        size_t currentIndex = 0;
        std::vector<unsigned char> buf(HID_REPORT_COUNT, 0);
        while (currentIndex < dataSize) {
            size_t remainingSize = dataSize - currentIndex;
            size_t chunkSizeToCopy = std::min(chunkSize, remainingSize);
            std::vector<unsigned char> chunk(chunkSizeToCopy + 3);
            for (size_t i = 0; i < chunkSizeToCopy; ++i) {
                chunk[i + 3] = static_cast<unsigned char>(message_str[currentIndex + i + add_size]);
            }
            auto scount = HID_REPORT_COUNT - chunk.size();
            if (scount > 0) {
                chunk.insert(chunk.end(), scount, 0);
            }
            chunk[0] = 0x4;
            chunk[1] = (currentIndex + chunkSize) < dataSize;
            chunk[2] = chunkSizeToCopy + 1;
            messageDatas.push_back(chunk);
            currentIndex += chunkSizeToCopy;
        }
    } else {
        // 不切分消息
        std::vector<unsigned char> buf(HID_REPORT_COUNT, 0);
        buf.assign(HID_REPORT_COUNT, 0);
        buf[0] = 0x4;
        buf[1] = 0;
        std::copy(message_str.begin(), message_str.end(), buf.begin() + 2);
        messageDatas.push_back(buf);
    }
    auto sss = message_size.data();
    auto sssize = message_size.size();
    for (auto &msg_data: messageDatas) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        int result_code = hid_write(dev, reinterpret_cast<const unsigned char *>(msg_data.data()),
                                    msg_data.size());
        if (result_code < 0) {
            send_mutex.unlock();
            auto err = hid_error(dev);
            // 处理发送失败的情况
            throw DeviceException(err);
        }
        result = result_code;
    }
    send_mutex.unlock();
    return result;
};

void HWDeviceTools::readDelimitedD2P(google::protobuf::io::ZeroCopyInputStream *rawInput,
                                     hid::msg::CtrlMessage *message) {
    // 创建一个新的编码流用于每个消息。
    google::protobuf::io::CodedInputStream input(rawInput);
    // 读取消息的大小。
    uint32_t size;
    if (!input.ReadVarint32(&size)) {
        throw DeviceException(L"readDelimitedFrom ReadSize 错误。");
    }
    // 设置流的大小限制。
    google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);
    // 解析消息。
    if (!message->MergeFromCodedStream(&input)) {
        throw DeviceException(L"readDelimitedFrom MergeFromCodedStream 错误。");
    }
    if (!input.ConsumedEntireMessage()) {
        throw DeviceException(L"readDelimitedFrom ConsumedEntireMessage 错误。");
    }
    // 释放大小限制。
    input.PopLimit(limit);
}

void HWDeviceTools::writeDelimitedP2D(const hid::msg::PcMessage &message,
                                      google::protobuf::io::ZeroCopyOutputStream *rawOutput,
                                      google::protobuf::io::ZeroCopyOutputStream *sizeOutput) {
    google::protobuf::io::CodedOutputStream output(rawOutput);
    // 写入消息的大小。
    const int size = message.ByteSizeLong();
    output.WriteVarint32(size + 1);
    if (sizeOutput != nullptr) {
        google::protobuf::io::CodedOutputStream sizeStream(sizeOutput);
        sizeStream.WriteVarint32(size);
    }
    uint8_t *buffer = output.GetDirectBufferForNBytesAndAdvance(size);
    if (buffer != nullptr) {
        // 优化：消息适合一个缓冲区，因此我们使用更快的直接序列化路径。
        message.SerializeWithCachedSizesToArray(buffer);
    } else {
        // 当消息跨多个缓冲区时，稍微慢一些。
        message.SerializeWithCachedSizes(&output);
        if (output.HadError()) {
            throw DeviceException(L"writeDelimitedTo SerializeWithCachedSizes 错误。");
        }
    }
}

}