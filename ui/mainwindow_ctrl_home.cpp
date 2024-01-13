//
// Created by 19254 on 2023/9/27.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "widget/MsgBox.hpp"
#include "utils/HttpApis.hpp"
#include "utils/config.hpp"
#include "utils/Firmware.hpp"

std::string checkCtrlLocalFirmwareVersion();

// 初始化
void MainWindow::ctrlHomeInit(QWidget *parent) {
  // checkFirmwareVersion();
  connect(ui->ctrl_home_update_mode_switch_gitee, &QPushButton::clicked, this,
          &MainWindow::ctrlHomeEventUpdateModeSwitchClicked);
  connect(ui->ctrl_home_update_mode_switch_github, &QPushButton::clicked, this,
          &MainWindow::ctrlHomeEventUpdateModeSwitchClicked);
  connect(ui->ctrl_home_button_check_ctrl_update, &QPushButton::clicked, this,
          &MainWindow::ctrlHomeEventGetNewVersion);
  connect(ui->ctrl_home_button_check_ctrl_get_update, &QPushButton::clicked, this,
          &MainWindow::ctrlHomeEventDownload);
  ui->ctrl_home_version_have_label_local->setText(checkCtrlLocalFirmwareVersion().c_str());

  auto cfg = GetConfigInstance();
  if (cfg->Ctrl.Firmware.UpdateSource == utils::HttpApis::FirmwareUpdataSource::Gitee) {
    ui->ctrl_home_update_mode_switch_gitee->setChecked(true);
    ui->ctrl_home_update_mode_switch_github->setChecked(false);
  } else if (cfg->Ctrl.Firmware.UpdateSource == utils::HttpApis::FirmwareUpdataSource::Github) {
    ui->ctrl_home_update_mode_switch_github->setChecked(true);
    ui->ctrl_home_update_mode_switch_gitee->setChecked(false);
  }
}

void MainWindow::ctrlHomeEventUpdateModeSwitchClicked(bool checked) {
  if (!checked) {
    return;
  }

  auto btn = qobject_cast<QRadioButton *>(sender());
  utils::HttpApis::FirmwareUpdataSource hostType = utils::HttpApis::FirmwareUpdataSource::Default;
  if (btn == ui->ctrl_home_update_mode_switch_gitee) {
    // 切换到码云源
    ui->ctrl_home_update_mode_switch_github->setChecked(false);
    hostType = utils::HttpApis::FirmwareUpdataSource::Gitee;
  } else if (btn == ui->ctrl_home_update_mode_switch_github) {
    // 切换到github源
    ui->ctrl_home_update_mode_switch_gitee->setChecked(false);
    hostType = utils::HttpApis::FirmwareUpdataSource::Github;
  } else {
    return;
  }
  auto cfg = GetConfigInstance();
  cfg->Ctrl.Firmware.UpdateSource = hostType;
  cfg.saveConfig();
}

// 检查固件版本事件
void MainWindow::ctrlHomeEventGetNewVersion(bool) {
  try {
    auto cfg = GetConfigInstance();
    auto firmwareLatestInfo = utils::HttpApis::GetCtrlFirmwareLatestInfo(
        utils::HttpApis::FirmwareUpdataSource(cfg->Ctrl.Firmware.UpdateSource));
    if (firmwareLatestInfo.last_assets.uf2.name.empty()) {
      ui->ctrl_home_version_have_label_new->setText("获取失败");
      return;
    }
    ui->ctrl_home_version_have_label_new->setText(firmwareLatestInfo.tag_name.c_str());
  } catch (...) {
    ui->ctrl_home_version_have_label_new->setText("获取失败");
  }
};

// 下载最新固件版本事件
void MainWindow::ctrlHomeEventDownload(bool) {
  try {
    ui->ctrl_home_button_check_ctrl_get_update->setEnabled(false);
    auto cfg = GetConfigInstance();
    auto firmwareLatestInfo = utils::HttpApis::GetCtrlFirmwareLatestInfo(
        utils::HttpApis::FirmwareUpdataSource(cfg->Ctrl.Firmware.UpdateSource));
    if (firmwareLatestInfo.last_assets.uf2.name.empty()
    || firmwareLatestInfo.last_assets.uf2.browser_download_url.empty()) {
      throw std::runtime_error("获取最新版失败");
    }
    QString firmwarePath = "firmware/ctrl";
    QString firmwareFilePath = firmwarePath + "/HelloWord-Dynamic-fw.uf2";
    // 检查路径是否存在
    QDir dir(firmwarePath);
    if (!dir.exists()) {
      dir.mkpath(firmwarePath);
    }
    // 检查文件是否存在
    if (QFile::exists(firmwareFilePath)) {
      // 弹窗确认
      auto ret = MsgBox::question(this, "提示", "固件已存在，是否覆盖？");
      if (ret != QMessageBox::Ok) {
        ui->ctrl_home_button_check_ctrl_get_update->setEnabled(true);
        return;
      }
      QFile::remove(firmwareFilePath);
    }
    ui->ctrl_home_version_have_label_new->setText(firmwareLatestInfo.tag_name.c_str());
    // 下载固件
    utils::HttpApis::DownloadFile(firmwareLatestInfo.last_assets.uf2.browser_download_url,
                                  firmwareFilePath.toStdString());
    MsgBox::information(this, "提示", "固件下载完成");
  } catch (const std::exception &e) {
    MsgBox::critical(this, "错误", QString::fromStdString(e.what()));
  }
  ui->ctrl_home_button_check_ctrl_get_update->setEnabled(true);
}


// 检查本地固件版本
std::string checkCtrlLocalFirmwareVersion() {
  QString firmwarePath = "firmware/ctrl";
  QString firmwareFilePath = firmwarePath + "/HelloWord-Dynamic-fw.uf2";
  QFile firmwareFile(firmwareFilePath);
  if (!firmwareFile.exists()) {
    return "未找到固件";
  }
  auto tagVer = utils::firmware::GetFirmwareTag(firmwareFilePath.toStdString(),
                                                "build_version_parse_tag:{{",
                                                "}}:build_version_parse_tag");
  return tagVer.empty() ? "未知版本" : tagVer;
}
