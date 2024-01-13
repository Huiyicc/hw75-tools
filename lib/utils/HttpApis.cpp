//
// Created by 19254 on 2023/12/30.
//

#include <filesystem>
#include "HttpApis.hpp"
#include "http/httplib.h"
#include "fmt/format.h"

namespace fs = std::filesystem;

namespace utils::HttpApis {

// 获取url带协议的域名文本
std::string GetUrlDomain(const std::string &url){
  auto lBegin = url.find("http://");
  if (lBegin == -1) {
    lBegin = url.find("https://");
    if (lBegin == -1) {
      return "";
    }
  }
  auto lEnd = url.find('/', lBegin + 8);
  if (lEnd == -1) {
    return "";
  }
  return url.substr(lBegin, lEnd - lBegin);
}

// 获取url的路径
std::string GetUrlPath(const std::string &url){
  auto lBegin = url.find("http://");
  if (lBegin == -1) {
    lBegin = url.find("https://");
    if (lBegin == -1) {
      return "";
    }
  }
  auto lEnd = url.find('/', lBegin + 8);
  if (lEnd == -1) {
    return "";
  }
  return url.substr(lEnd);
}

HttpApis::FirmwareInfo GetCtrlFirmwareLatestInfo(FirmwareUpdataSource hostType) {
  std::string apiHost;
  std::string apiPath;
  switch (hostType) {
    case FirmwareUpdataSource::Gitee:
      // gitee
      apiHost = "https://gitee.com";
      apiPath = "/api/v5/repos/LoveA/HelloWord_HY/releases/latest";
      break;
    case FirmwareUpdataSource::Github:
      // github
      apiHost = "https://api.github.com";
      apiPath = "/repos/Huiyicc/HelloWord_HY/releases/latest";
      break;
    case FirmwareUpdataSource::Default:
    default:
      // gitee
      apiHost = "https://gitee.com";
      apiPath = "/api/v5/repos/LoveA/HelloWord_HY/releases/latest";
      break;
  }
  // 获取api
  httplib::Client cli(apiHost);
  auto resp = cli.Get(apiPath);
  if (resp->status != 200) {
    throw std::runtime_error("获取仓库信息错误!");
  }
  FirmwareInfo lobj;
  lobj.FromJson(nlohmann::json::parse(resp->body));
  for (auto &i: lobj.assets) {
    if (i.name.size() < 3) continue;
    if (i.name.find("Dynamic") == -1) continue;
    auto p = i.name.substr(i.name.length() - 3);
    if (p == "uf2") {
      lobj.last_assets.uf2 = i;
    } else if (p == "bin") {
      lobj.last_assets.bin = i;
    } else if (p == "hex") {
      lobj.last_assets.hex = i;
    }
  }
  if (lobj.last_assets.uf2.name.empty() || lobj.last_assets.uf2.browser_download_url.empty()
      || lobj.last_assets.bin.name.empty() || lobj.last_assets.bin.browser_download_url.empty()
      || lobj.last_assets.hex.name.empty() || lobj.last_assets.hex.browser_download_url.empty()
  ) {
    throw std::runtime_error("获取扩展固件最新版本信息错误!");
  }
  return lobj;
}

// 下载文件
void DownloadFile(const std::string &url, const std::string &path) {
  // 检查文件是否存在
  if (fs::exists(path)) {
    fs::remove(path);
  }
  auto lDomain = GetUrlDomain(url);
  auto lPath = GetUrlPath(url);
  httplib::Client cli(lDomain);
  cli.set_follow_location(true);
  std::ofstream outputFile(path, std::ios::app);
  if (!outputFile.is_open()) {
    throw std::runtime_error(fmt::format("打开文件失败!"));
  }
  auto resp = cli.Get(lPath,[&](const char *data, size_t data_length) {
    outputFile.write(data, data_length);
    return true;
  });
  outputFile.close();
  if (resp->status != 200) {
    throw std::runtime_error(fmt::format("下载文件失败!"));
  }
}
}
