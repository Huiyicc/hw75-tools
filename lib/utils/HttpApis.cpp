//
// Created by 19254 on 2023/12/30.
//

#include "HttpApis.hpp"
#include "http/httplib.h"
#include "fmt/format.h"

namespace utils {
HttpApis::FirmwareInfo HttpApis::GetFirmwareLatestInfo(int hostType) {
  std::string apiHost;
  std::string apiPath;
  switch (hostType) {
    case 1:
      // gitee
      apiHost = "https://gitee.com";
      apiPath = "/api/v5/repos/LoveA/HelloWord_HY/releases/latest";
      break;
    case 2:
      // github
      apiHost = "https://api.github.com";
      apiPath = "/repos/Huiyicc/HelloWord_HY/releases/latest";
      break;
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
}
