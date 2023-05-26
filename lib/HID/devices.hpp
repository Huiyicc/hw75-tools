//
// Created by HuiYi on 2023/5/22.
//

#ifndef HW_TOOLS_DEVICES_HPP
#define HW_TOOLS_DEVICES_HPP

#include <stdexcept>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <hidapi/hidapi.h>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <QMetaType>
#include "protobuf/usb_comm.pb.h"
#include "protobuf/uart_comm.pb.h"

inline std::vector<uint8_t> unhexlify(const std::string &hex_string) {
	std::vector<uint8_t> bytes;

	for (size_t i = 0; i < hex_string.length(); i += 2) {
		std::string byteString = hex_string.substr(i, 2);
		uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
		bytes.push_back(byte);
	}

	return bytes;
}


namespace Lib {

	struct HWDevice {
		/** 设备路径 */
		QString Path;

		/** 设备ID */
		unsigned short VendorId = 0;

		/** 产品ID */
		unsigned short ProductId = 0;

		/** 设备名称 */
		QString ProductName;

		/** 这是一个表示逻辑设备的USB接口。仅当该设备为USB HID设备时有效。在其他情况下设为-1。 */
		int InterfaceNumber = 0;

		/** 这个设备/接口的用法。 */
		unsigned short Usage = 0;

		/** 使用页 */
		unsigned short UsagePage = 0;

		/** 序列号 */
		QString SerialNumber;
	};

	struct HWDeviceDynamicFeatures {
		bool Rgb = false;
		bool Eink = true;
		bool Knob = true;
		bool KnobPrefs = true;
		bool RgbFullControl = true;
		bool RgbIndicator = true;
	};

	struct HWDeviceDynamicVersion {
		QString AppVersion;
		QString ZmkVersion;
		QString ZephyrVersion;
		HWDeviceDynamicFeatures Features;
	};


	// USB设备管理器的错误类
	class DeviceException : public std::runtime_error {
	public:
		explicit DeviceException(const std::wstring &message) : std::runtime_error(w2a(message.c_str())) {}

		explicit DeviceException(const wchar_t *message) : std::runtime_error(w2a(message)) {}

	private:
		static std::string w2a(const wchar_t *str) {
			// 将wchar_t*转换为std::string
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			return converter.to_bytes(str);
		};
	};

	// 瀚文HID管理器类
	// 所有函数都有错误处理,错误抛出 Lib::HID::DeviceException
	// 构造时会自动初始化,注意捕获异常
	class HWDeviceTools {
	public:
		HWDeviceTools();

		~HWDeviceTools();

		/** 获取瀚文设备列表(暂时只能获取扩展组件) */
		size_t GetHWDevicesList(std::vector<HWDevice> &HWDevicesList);

		HWDeviceDynamicVersion GetVersion(HWDevice &devices);


	private:
		constexpr static int HWVID = 0x1d50;
		constexpr static int HWPID = 0x615e;
		constexpr static int USB_COMM_USAGE_PAGE = 0xff14;
		constexpr static int HID_COMM_REPORT_COUNT = 63;
		constexpr static int USB_COMM_PAYLOAD_SIZE = HID_COMM_REPORT_COUNT - 1;

		void readDelimitedD2H(google::protobuf::io::ZeroCopyInputStream *rawInput,
		                      usb::comm::MessageD2H *message);

		void writeDelimitedH2D(const usb::comm::MessageH2D &message,
		                       google::protobuf::io::ZeroCopyOutputStream *rawOutput);

	};

}

#endif //HW_TOOLS_DEVICES_HPP
