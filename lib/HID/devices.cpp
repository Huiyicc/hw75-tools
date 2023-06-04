//
// Created by HuiYi on 2023/5/22.
//
#include "devices.hpp"
#include <sstream>
#include "utils/math.hpp"

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
		HWDevicesList.clear();
		// 获取连接的HID设备列表
		auto devs = hid_enumerate(0x0, 0x0);
		auto cur_dev = devs;

		// 开始遍历设备列表
		while (cur_dev) {
			// 判断设备是否为USB HID设备

			if (cur_dev->product_id == HWPID && cur_dev->vendor_id == HWVID // 判断是否为瀚文设备
			    && cur_dev->usage_page == 65300
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
		return HWDevicesList.size();
	};

	HWDeviceDynamicVersion HWDeviceTools::GetDynamicVersion(HWDevice &devices) {
		HWDeviceDynamicVersion result;
		auto device = hid_open_path(devices.Path.toStdString().c_str());
		if (!device) {
			throw DeviceException(hid_error(nullptr));
		}
		usb::comm::MessageH2D message;
		message.set_action(usb::comm::Action::VERSION);
		usb::comm::Nop *nop = message.mutable_nop();
		// 发送消息
		int result_code = sendMessage(device, message);

		// 读取USB设备返回的字节流
		std::shared_ptr<uint8_t> data(new uint8_t[result_code]);
		// 处理返回的字节流
		usb::comm::MessageD2H messageD2H;
		readMessage(device, messageD2H, result_code);
		// 判断返回的消息是否为版本消息
		if (!messageD2H.has_version()) {
			throw DeviceException(L"message has no version.");
		}
		result.ZephyrVersion = QString::fromStdString(messageD2H.version().zephyr_version());
		result.ZmkVersion = QString::fromStdString(messageD2H.version().zmk_version());
		result.AppVersion = QString::fromStdString(messageD2H.version().app_version());
		result.Features.Rgb = messageD2H.version().features().rgb();
		result.Features.Eink = messageD2H.version().features().eink();
		result.Features.Knob = messageD2H.version().features().knob();
		result.Features.KnobPrefs = messageD2H.version().features().knob_prefs();
		result.Features.RgbFullControl = messageD2H.version().features().rgb_full_control();
		result.Features.RgbIndicator = messageD2H.version().features().rgb_indicator();
		// 关闭设备
		hid_close(device);
		return result;

	};

	void HWDeviceTools::SetDynamicScerrn(HWDevice &devices, QByteArray &imageArrar) {
		SetDynamicScerrn(devices.Path,imageArrar);
	};
	void HWDeviceTools::SetDynamicScerrn(const QString &devicesPath, QByteArray &imageArrar) {
		HWDeviceDynamicVersion result;
		auto device = hid_open_path(devicesPath.toStdString().c_str());
		if (!device) {
			throw DeviceException(hid_error(nullptr));
		}
		usb::comm::MessageH2D message;
		message.set_action(usb::comm::Action::EINK_SET_IMAGE);
		// 创建EinkImage消息对象
		usb::comm::EinkImage *einkImage = message.mutable_eink_image();
		einkImage->set_id(Lib::RandomNumber(0, 1000));
		//einkImage->set_bits_length(imageArrar.size());
		einkImage->set_bits(imageArrar.data(), imageArrar.size());
		sendMessage(device, message);

		// 关闭设备
		hid_close(device);

	};

	int HWDeviceTools::readMessage(hid_device_ *dev, usb::comm::MessageD2H &message,int messageSize) {
// 读取USB设备返回的字节流
		std::shared_ptr<uint8_t> data(new uint8_t[messageSize]);

		messageSize = hid_read(dev, data.get(), messageSize);
		if (messageSize < 0) {
			auto err = hid_error(dev);
			// 处理发送失败的情况
			throw DeviceException(err);
		}
		//第一个字节删除,第二个字节为数据长度
		int lents = data.get()[1];
		if (lents == USB_COMM_PAYLOAD_SIZE) {
			throw DeviceException(L"data lens is to long.");
		}
		// 根据长度写入流
		google::protobuf::io::ArrayInputStream arrayInput((void *) ((long long) (&data.get()[2])), lents);
		// 解码字节流
		readDelimitedD2H(&arrayInput, &message);
		return messageSize;
	};

	int HWDeviceTools::sendMessage(hid_device_ *dev, usb::comm::MessageH2D &message) {
		int result = 0;
		//序列化消息
		std::string message_str;
		google::protobuf::io::StringOutputStream stringOutput(&message_str);
		writeDelimitedH2D(message, &stringOutput);

		std::vector<unsigned char> buf;
		std::vector<unsigned char> out(HID_COMM_REPORT_COUNT, 0);
		for (size_t i = 0; i <= message_str.size(); i += USB_COMM_PAYLOAD_SIZE) {
			size_t end = std::min(i + USB_COMM_PAYLOAD_SIZE, message_str.size());
			buf.assign(message_str.begin() + i, message_str.begin() + end);
			out.assign(HID_COMM_REPORT_COUNT, 0);
			out[0] = 1;
			out[1] = buf.size();
			std::copy(buf.begin(), buf.end(), out.begin() + 2);
			// 将字节流发送到USB设备
			int result_code = hid_write(dev, reinterpret_cast<const unsigned char *>(out.data()),
			                            out.size());
			if (result_code < 0) {
				auto err = hid_error(dev);
				// 处理发送失败的情况
				throw DeviceException(err);
			}
			result = result_code;
		}
		return result;
	};

	void HWDeviceTools::readDelimitedD2H(google::protobuf::io::ZeroCopyInputStream *rawInput,
	                                     usb::comm::MessageD2H *message) {
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

	void HWDeviceTools::writeDelimitedH2D(const usb::comm::MessageH2D &message,
	                                      google::protobuf::io::ZeroCopyOutputStream *rawOutput) {
		google::protobuf::io::CodedOutputStream output(rawOutput);
		// 写入消息的大小。
		const int size = message.ByteSizeLong();
		output.WriteVarint32(size);
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