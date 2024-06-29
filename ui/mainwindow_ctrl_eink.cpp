//
// Created by 19254 on 2023/9/27.
//
#include "./ui_mainwindow.h"
#include "fmt/format.h"
#include "mainwindow.h"
#include "ui/widget/MsgBox.hpp"
#include "utils/Conv.hpp"
#include "utils/Firmware.hpp"
#include "utils/Log.hpp"
#include "utils/config.hpp"
#include "utils/file.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include "utils/defer.hpp"

// 初始化
void MainWindow::ctrlEinkInit(QWidget *parent) {
  connect(ui->ctrl_eink_button_switchImage, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkSwitchImage);
  connect(ui->ctrl_eink_image_threshold_horizontalSlider, &QSlider::valueChanged, this,
          &MainWindow::ctrlEventEinkValueChanged);
  connect(ui->ctrl_eink_button_pushImage, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkPushImage);
  connect(ui->ctrl_eink_button_save_firmware, &QPushButton::clicked, this, &MainWindow::ctrlEventEinkSaveFirmware);
  ui->ctrl_eink_imgedit_group->setVisible(false);
}

// 选择图片
void MainWindow::ctrlEventEinkSwitchImage(bool checked) {
  // 选择图片按钮事件
  // 创建文件选择对话框
  QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "图像文件 (*.jpg *.jpeg *.png *.bmp)");
  ;
  // 检查用户是否选择了文件
  if (filePath.isEmpty()) {
    return;
  }
  ui->ctrl_eink_button_switchImage->setVisible(!filePath.isEmpty());
  ui->ctrl_eink_imgview->clear();
  ui->ctrl_eink_edit_imagePath->setText(filePath);
  // 设置预览图像
  auto tmpptr = std::make_shared<Lib::Image>(filePath);
  m_userPushImage = tmpptr;
  auto img = m_userPushImage->GetImage();
  ui->ctrl_eink_image_threshold_horizontalSlider->setMinimum(1);
  //ui->ctrl_eink_image_threshold_horizontalSlider->setMaximum(m_userPushImage->GetMaximumThreshold());
  ui->ctrl_eink_image_threshold_horizontalSlider->setMaximum(255);
  ui->ctrl_eink_image_threshold_label->setText("50");
  ui->ctrl_eink_image_threshold_horizontalSlider->setValue(50);
  ui->ctrl_eink_imgedit_group->setEnabled(true);
  ui->ctrl_eink_imgedit_group->setVisible(true);
  ctrlEventEinkValueChanged(50);
}

void MainWindow::ctrlEinkUpdateUserPreviewImage() {
  QImage scaledimg;
  std::shared_ptr<QImage> img = m_userPushImage->GetBinaryImgData(
      ui->ctrl_eink_image_threshold_horizontalSlider->value());
  scaledimg = img->scaled(ui->ctrl_eink_imgview->width(), ui->ctrl_eink_imgview->height(), Qt::KeepAspectRatio);
  ui->ctrl_eink_imgview->setPixmap(QPixmap::fromImage(scaledimg));
}

void MainWindow::ctrlEventEinkValueChanged(int value) {
  // 阈值条拖动之后事件
  ui->ctrl_eink_image_threshold_label->setText(QString::number(value));
  ctrlEinkUpdateUserPreviewImage();
}

void MainWindow::ctrlEventEinkPushImage(bool checked) {
  // 更新屏幕图片按钮事件
  try {
    if (!checkCtrlConnect()) {
      return;
    }
    ui->ctrl_eink_button_pushImage->setEnabled(false);
    ui->ctrl_eink_button_pushImage->setText("推送中");
    Lib::HWDeviceTools tools;

    tools.SetDynamicEinkScerrn(getCtrlConnectDev(), *m_userPushImage->BinaryImgDataToBits(
                                                             ui->ctrl_eink_image_threshold_horizontalSlider->value()));
    QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
  } catch (Lib::DeviceException &e) {
    QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  }
  ui->ctrl_eink_button_pushImage->setEnabled(true);
  ui->ctrl_eink_button_pushImage->setText("推送图片");
}

bool MainWindow::ctrlEinkPushImage(const char *data, int len) {
  try {
    if (!checkCtrlConnect()) {
      // 未连接设备
      return false;
    }
    // 创建图像对象
    QByteArray byteArray(data, len);
    auto imgLib = Lib::Image(QImage::fromData(byteArray));
    Lib::HWDeviceTools tools;
    tools.SetDynamicEinkScerrn(getCtrlConnectDev(), *imgLib.BinaryImgDataToBits(155));
    // QMessageBox::information(this, "信息", QString::fromStdString("推送成功"));
    return true;
  } catch (Lib::DeviceException &e) {
    // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  }
  return false;
}

constexpr const char resources_data_tage_begin[] = {'H', 'Y', 'S', 'P', 'L', 'I', '{', '{', '\0'};
constexpr const char resources_data_tage_end[] = {'}', '}', 'L', 'I', 'P', 'S', 'Y', 'H', '\0'};

std::string einkSaveFirmware(QWidget *parent, std::shared_ptr<Lib::Image> ImgPtr, int threshold = 155) {
  QString firmwarePath = QCoreApplication::applicationDirPath() + "/firmware/ctrl";
  PrintDebug("Firmware Path: {}", firmwarePath.toStdString());
  QString firmwareFilePath = firmwarePath + "/HelloWord-Dynamic-fw.bin";
  QFile firmwareFile(firmwareFilePath);
  if (!firmwareFile.exists()) {
    throw std::runtime_error("获取本地固件失败");
  }
  // 选择保存路径
  QString savePath = QFileDialog::getSaveFileName(parent,
                                                  "保存固件",
                                                  fmt::format("./firmware/ctrl/{}.uf2", utils::files::FileExtractName(firmwareFilePath.toStdString(), false)).c_str(),
                                                  "UF2 固件 (*.uf2);;Bin固件 (*.bin)");
  if (savePath.isEmpty()) {
    // 用户取消
    return "0";
  }
  // 防呆检查
  // 检查后缀
  auto ext = utils::files::FileExtractExtension(savePath.toStdString());
  if (ext != ".uf2" && ext != ".bin") {
    savePath += ".uf2";
    ext = ".uf2";
  } else if (ext == ".uf2") {
    //    // 检查转换器
    //    auto uf2convPath = QCoreApplication::applicationDirPath() + "/uf2conv.exe";
    //    if (!QFile::exists(uf2convPath)) {
    //      throw std::runtime_error("找不到uf2conv转换器,请检查程序是否完整");
    //    }
  }

  // 读入文件(文件为二进制)
  // std::ifstream file(firmwareFilePath.toStdString(), std::ios::binary);
  //  if (!file.is_open()) {
  //    throw std::runtime_error(fmt::format("打开固件 {} 失败", firmwareFilePath.toStdString()));
  //  }
  std::string BeginTag = resources_data_tage_begin, EndTag = resources_data_tage_end;
//  std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//  auto file = fopen(firmwareFilePath.toStdString().c_str(), "rb");
//  if (file == nullptr) {
//    throw std::runtime_error(fmt::format("打开固件 {} 失败", firmwareFilePath.toStdString()));
//  }
//  DEFER(fclose(file));
  auto qData = utils::files::ReadFile(firmwareFilePath);
  std::string fileContent = qData.toStdString();
  if (fileContent.size() != qData.size()){
    throw std::runtime_error(fmt::format("读取固件 {} 失败", firmwareFilePath.toStdString()));
  }
  size_t beginPos = fileContent.find(BeginTag);
  if (beginPos == std::string::npos) {
    std::cerr << "BeginTag not found in file." << std::endl;
    throw std::runtime_error("解析固件错误,错误位置:BeginTag");
  }

  beginPos += BeginTag.length();

  size_t endPos = fileContent.find(EndTag, beginPos);
  if (endPos == std::string::npos) {
    std::cerr << "EndTag not found in file." << std::endl;
    throw std::runtime_error("解析固件错误,错误位置:EndTag");
  }
  auto oldData = fileContent.substr(beginPos, endPos - beginPos);

  if (oldData.size() != (Lib::Image::EPD_HEIGHT * Lib::Image::EPD_WIDTH / 8)) {
    throw std::runtime_error("解析固件错误");
  }
  // 图片取模
  auto data = *ImgPtr->BinaryImgDataToBits(threshold);
  fileContent.replace(beginPos, endPos - beginPos, (char *) data.data(), data.size());
  if (ext == ".uf2") {
    //uf2需要转换
    fileContent = Lib::Convert::BinToUf2(fileContent);
  }
  // 写入文件
  QFile saveFile(savePath);
  if (!saveFile.open(QIODevice::WriteOnly)) {
    throw std::runtime_error(fmt::format("保存固件到 {} 失败", savePath.toStdString()));
  }
  DEFER(saveFile.close());
  saveFile.write(QByteArray::fromStdString(fileContent));
  return savePath.toStdString();
};


void MainWindow::ctrlEventEinkSaveFirmware(bool checked) {
  // 保存固件按钮事件
  try {
    if (!checkCtrlConnect()) {
      return;
    }
    std::string localVer;
    if (!CheckCtrlLocalFirmwareVersion(localVer)) {
      throw std::runtime_error("获取本地固件失败");
    }
    auto reFile = einkSaveFirmware(nullptr, m_userPushImage, ui->ctrl_eink_image_threshold_horizontalSlider->value());
    if (!reFile.empty()) {
      if (reFile!="0") {
        MsgBox::information(this, "信息", QString::fromStdString(fmt::format("保存固件到 {} 成功", reFile)));
      }
    } else {
      MsgBox::critical(this, "错误", QString::fromStdString("保存失败"));
    }
  } catch (Lib::DeviceException &e) {
    QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  } catch (std::exception &e) {
    QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
  }
};
