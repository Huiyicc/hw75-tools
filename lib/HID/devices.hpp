//
// Created by HuiYi on 2023/5/22.
//

#ifndef HW_TOOLS_DEVICES_HPP
#define HW_TOOLS_DEVICES_HPP

#include "image/Image.hpp"
#include "nlohmann/json.hpp"
#include "protobuf/hid_msg.pb.h"
#include <QMetaType>
#include <algorithm>
#include <codecvt>
#include <cstring>
#include <fstream>
#include <hidapi/hidapi.h>
#include <locale>
#include <stdexcept>
#include <string>
#include <vector>

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
        {"serial_number", SerialNumber.toStdString()}};
  }
};

struct KnobAppConf {
  // 电机模式
  int Mode = 0;
  int AddedValue = 0;
  struct ConfigData {
    // 当前值
    float value = 0;
    // 最小值
    float min = 0;
    // 最大值
    float max = 0;
  }
  // 力矩限制
  torqueLimitConf,
      // 速度限制
      velocityLimitConf,
      // 步数
      stepConf;

  nlohmann::json toJson() {
    return {
        {"mode", Mode},
        {"added_value", AddedValue},
        {"torque_limitConf", {"value", torqueLimitConf.value, "min", torqueLimitConf.min, "max", torqueLimitConf.max}},
        {"velocity_limitConf", {"value", velocityLimitConf.value, "min", velocityLimitConf.min, "max", velocityLimitConf.max}},
        {"step_conf", {"value", stepConf.value, "min", stepConf.min, "max", stepConf.max}}};
  }
};

struct CtrlSysCfg {
  // 休眠时间(单位:秒)
  uint32_t SleepTime = 0;
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
  QString GitHash;
  QString GitBranch;
  QString GitVersion;
  QString BuildVersion;
  HWDeviceDynamicFeatures Features;

  nlohmann::json toJson() {
    return {
        {"git_version", GitVersion.toStdString()},
        {"git_bash", GitBranch.toStdString()},
        {"git_hash", GitHash.toStdString()},
        {"build_version", BuildVersion.toStdString()},
        {"features", {{"rgb", Features.Rgb}, {"eink", Features.Eink}, {"knob", Features.Knob}, {"knob_prefs", Features.KnobPrefs}, {"rgb_full_control", Features.RgbFullControl}, {"rgb_indicator", Features.RgbIndicator}}}};
  }
};

struct HWDeviceDynamicKnobStatus {
  // 当前模式
  uint32_t model = 0;
  // 当前角度
  float current_angle = 0;
  // 目标角度
  float target_angle = 0;
  // 当前速度
  float current_velocity = 0;
  // 目标速度
  float target_velocity = 0;
  // 当前电压
  float target_voltage = 0;
};

struct HWDeviceDynamicRGBStatus {
  // 当前模式
  uint32_t model = 0;
  // 当前颜色
  uint8_t color_r = 0;
  uint8_t color_g = 0;
  uint8_t color_b = 0;
  // 当前亮度
  float brightness = 0;
  // 休眠熄灯
  bool sleep_off = false;
  // 休眠后亮度
  float sleep_brightness = 0;
  nlohmann::json toJson() {
    return {
        {"model", model},
        {"color_r", color_r},
        {"color_g", color_g},
        {"color_b", color_b},
        {"brightness", brightness},
        {"sleep_off", sleep_off},
        {"sleep_brightness", sleep_brightness}};
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
  static size_t GetHWDevicesList(std::vector<HWDevice> &HWDevicesList);

  /** 获取扩展模块的版本号信息 */
  HWDeviceDynamicVersion GetDynamicVersion(HWDevice &devices);

  //---
  /** 设置扩展模块的屏幕
     * 注意: 屏幕尺寸只有296*128, 且只能显示黑白,输入图像注意处理
     * */
  void SetDynamicEinkScerrn(HWDevice &devices, std::vector<unsigned char> &imageArrar);

  void SetDynamicEinkScerrn(const QString &devicesPath, std::vector<unsigned char> &imageArrar);

  //---
  // 获取扩展App配置
  KnobAppConf GetDynamicAppinConf(HWDevice &devices, int appId);

  KnobAppConf GetDynamicAppinConf(const QString &devicesPath, int appId);

  //---
  // 设置扩展App配置
  void SetDynamicAppinConf(HWDevice &devices, int appId, hid::msg::SetAppType setAppType, KnobAppConf &conf);

  void SetDynamicAppinConf(const QString &devicesPath, int appId, hid::msg::SetAppType setAppType, KnobAppConf &conf);
  //---
  // 获取扩展RGB配置
  HWDeviceDynamicRGBStatus GetRgbConfig(const HWDevice &devices, int id);
  void SetDynamicRgbConfig(const HWDevice &devices, int id, const HWDeviceDynamicRGBStatus &conf);

  //---
  // 获取扩展系统配置
  CtrlSysCfg GetDynamicSysConf(HWDevice &devices);
  // 设置扩展系统设置
  void SetDynamicSysConf(HWDevice &devices, const CtrlSysCfg &conf);
  //---
  /** 设置扩展模块的OLED
    * 注意: 屏幕尺寸只有32*128, 且只能显示黑白,输入图像注意处理
     * */
  void SetDynamicOLEDScerrn(HWDevice &devices, std::vector<unsigned char> &imageArrar);

  void SetDynamicOLEDScerrn(const QString &devicesPath, std::vector<unsigned char> &imageArrar);

  //---
  // 设置扩展屏幕
  void SetDynamicScerrn(int id, const QString &devicesPath, std::vector<unsigned char> &imageArrar);
  //---
  HWDeviceDynamicKnobStatus GetKnobStatus(const HWDevice &devices);

private:
  constexpr static int HWVID = 0xdc00;
  constexpr static int HWPID = 0x5750;
  constexpr static int USB_USAGE_PAGE = 0x8c;
  constexpr static int USB_USAGE_PAGE_PLUGIN = 0x06;
  constexpr static int HID_REPORT_COUNT = 64;
  constexpr static int HID_PAYLOAD_SIZE = HID_REPORT_COUNT - 3;

  // 发送消息,外部调用禁止上锁
  int sendMessage(hid_device_ *dev, hid::msg::PcMessage &message, bool lock = true);

  int readMessage(hid_device_ *dev, hid::msg::CtrlMessage &message, int messageSize);

  void readDelimitedD2P(google::protobuf::io::ZeroCopyInputStream *rawInput,
                        hid::msg::CtrlMessage *message);

  void writeDelimitedP2D(const hid::msg::PcMessage &message,
                         google::protobuf::io::ZeroCopyOutputStream *rawOutput,
                         google::protobuf::io::ZeroCopyOutputStream *sizeOutput = nullptr);
};

}// namespace Lib

Q_DECLARE_METATYPE(Lib::HWDevice);

#endif//HW_TOOLS_DEVICES_HPP
