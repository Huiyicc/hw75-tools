//
// Created by 19254 on 2023/12/30.
//

#ifndef HW_TOOLS_HTTPAPIS_HPP
#define HW_TOOLS_HTTPAPIS_HPP

#include <iostream>

#include "nlohmann/json.hpp"

namespace utils {


namespace HttpApis {

enum FirmwareUpdataSource {
  Default = 0,
  Gitee = 1,
  Github = 2,
};

struct Assets {
  std::string browser_download_url;
  std::string name;

  void FromJson(const nlohmann::json &json) {
    this->browser_download_url = json.value<std::string>("browser_download_url", "");
    this->name = json.value<std::string>("name", "");
  }
};

struct Author {
  int id = 0;
  std::string login;
  std::string name;
  std::string avatar_url;
  std::string url;
  std::string html_url;
  std::string remark;
  std::string followers_url;
  std::string following_url;
  std::string gists_url;
  std::string starred_url;
  std::string subscriptions_url;
  std::string organizations_url;
  std::string repos_url;
  std::string events_url;
  std::string received_events_url;
  std::string type;

  void FromJson(const nlohmann::json &json) {
    this->id = json.value<int>("id", 0);
    this->login = json.value<std::string>("login", "");
    this->name = json.value<std::string>("name", "");
    this->avatar_url = json.value<std::string>("avatar_url", "");
    this->url = json.value<std::string>("url", "");
    this->html_url = json.value<std::string>("html_url", "");
    this->remark = json.value<std::string>("remark", "");
    this->followers_url = json.value<std::string>("followers_url", "");
    this->following_url = json.value<std::string>("following_url", "");
    this->gists_url = json.value<std::string>("gists_url", "");
    this->starred_url = json.value<std::string>("starred_url", "");
    this->subscriptions_url = json.value<std::string>("subscriptions_url", "");
    this->organizations_url = json.value<std::string>("organizations_url", "");
    this->repos_url = json.value<std::string>("repos_url", "");
    this->events_url = json.value<std::string>("events_url", "");
    this->received_events_url = json.value<std::string>("received_events_url", "");
    this->type = json.value<std::string>("type", "");
  }
};

struct FirmwareInfo {
  int id = 0;
  std::string tag_name;
  std::string target_commitish;
  bool prerelease = false;
  std::string name;
  std::string body;
  std::string created_at;
  std::vector<Assets> assets;
  Author author;
  struct _ {
    Assets bin;
    Assets hex;
    Assets uf2;
  } last_assets;

  void FromJson(const nlohmann::json &json) {
    this->id = json.value<int>("id", 0);
    this->tag_name = json.value<std::string>("tag_name", "");
    this->target_commitish = json.value<std::string>("target_commitish", "");
    this->prerelease = json.value<bool>("prerelease", false);
    this->name = json.value<std::string>("name", "");
    this->body = json.value<std::string>("body", "");
    this->created_at = json.value<std::string>("created_at", "");
    this->author.FromJson(json["author"]);
    for (auto &i: json["assets"]) {
      Assets assObj;
      assObj.FromJson(i);
      this->assets.push_back(assObj);
    }
  }
};

// 获取最新固件版本信息
FirmwareInfo GetCtrlFirmwareLatestInfo(FirmwareUpdataSource hostType);

// 下载文件
void DownloadFile(const std::string &url, const std::string &path);

};// namespace HttpApis
}// namespace utils

#endif//HW_TOOLS_HTTPAPIS_HPP
