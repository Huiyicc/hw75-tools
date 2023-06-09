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
#include "image/Image.hpp"
#include "nlohmann/json.hpp"

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

		nlohmann::json toJson() {
			return {
					{"path", Path.toStdString()},
					{"vendor_id", VendorId},
					{"product_id", ProductId},
					{"product_name", ProductName.toStdString()},
					{"interface_number", InterfaceNumber},
					{"usage", Usage},
					{"usage_page", UsagePage},
					{"serial_number", SerialNumber.toStdString()}
			};
		}
	};

	struct HWDeviceDynamicFeatures {
		bool Rgb = false;
		bool Eink = false;
		bool Knob = false;
		bool KnobPrefs = false;
		bool RgbFullControl = false;
		bool RgbIndicator = false;
	};

	struct HWDeviceDynamicVersion {
		QString AppVersion;
		QString ZmkVersion;
		QString ZephyrVersion;
		HWDeviceDynamicFeatures Features;
		nlohmann::json toJson() {
			return {
					{"app_version", AppVersion.toStdString()},
					{"zmk_version", ZmkVersion.toStdString()},
					{"zephyr_version", ZephyrVersion.toStdString()},
					{"features", {
						{"rgb", Features.Rgb},
						{"eink", Features.Eink},
						{"knob", Features.Knob},
						{"knob_prefs", Features.KnobPrefs},
						{"rgb_full_control", Features.RgbFullControl},
						{"rgb_indicator", Features.RgbIndicator}
					}}
			};
		}
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

		/** 获取扩展模块的版本号信息 */
		HWDeviceDynamicVersion GetDynamicVersion(HWDevice &devices);

		/** 设置扩展模块的屏幕
		 * 注意: 屏幕尺寸只有296*128, 且只能显示黑白,输入图像注意处理
		 * */
		void SetDynamicScerrn(HWDevice &devices, QByteArray &imageArrar);
		void SetDynamicScerrn(const QString &devicesPath, QByteArray &imageArrar);

	private:
		constexpr static int HWVID = 0x1d50;
		constexpr static int HWPID = 0x615e;
		constexpr static int USB_COMM_USAGE_PAGE = 0xff14;
		constexpr static int HID_COMM_REPORT_COUNT = 64;
		constexpr static int USB_COMM_PAYLOAD_SIZE = HID_COMM_REPORT_COUNT - 2;

		int sendMessage(hid_device_ *dev, usb::comm::MessageH2D &message);
		int readMessage(hid_device_ *dev, usb::comm::MessageD2H &message,int messageSize);

		void readDelimitedD2H(google::protobuf::io::ZeroCopyInputStream *rawInput,
		                      usb::comm::MessageD2H *message);

		void writeDelimitedH2D(const usb::comm::MessageH2D &message,
		                       google::protobuf::io::ZeroCopyOutputStream *rawOutput);

	};

}

#endif //HW_TOOLS_DEVICES_HPP
