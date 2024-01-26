//
// Created by 19254 on 2023/9/27.
//
#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "widget/MsgBox.hpp"
#include <QFileDialog>
#include <QMessageBox>

auto getRgbStatusFunc = [](const Lib::HWDevice &dev, int index,
                           QRadioButton *sw1, QRadioButton *sw2, QRadioButton *sw3,
                           QSlider *horizontal, QLabel *hValue,
                           QSlider *r, QLabel *rValue,
                           QSlider *g, QLabel *gValue,
                           QSlider *b, QLabel *bValue
                           ) {
  Lib::HWDeviceTools tools;
  auto result = tools.GetRgbConfig(dev, index);
  QRadioButton *swp = nullptr;
  QRadioButton *swps[2] = {nullptr, nullptr};
  switch (result.model) {
    case 0: {
      swp = sw1;
      swps[0] = sw2;
      swps[1] = sw3;
    } break;
    case 1: {
      swp = sw2;
      swps[0] = sw1;
      swps[1] = sw3;
    } break;
    case 2: {
      swp = sw3;
      swps[0] = sw1;
      swps[1] = sw2;
    } break;
    default:
      return;
  }
  swp->setChecked(true);
  swps[0]->setChecked(false);
  swps[1]->setChecked(false);

  // 亮度
  // 0~1,进度条百位
  horizontal->setValue(result.brightness * 100);
  hValue->setText(QString::number(result.brightness, 'f', 2));
  //rgb
  r->setValue(result.color_r);
  rValue->setText(QString::number(result.color_r));
  g->setValue(result.color_g);
  gValue->setText(QString::number(result.color_g));
  b->setValue(result.color_b);
  bValue->setText(QString::number(result.color_b));
};

// 初始化
void MainWindow::ctrlRgbInit(QWidget *parent) {
  ui->ctrl_RGB_mood_groupBox->setVisible(false);
  ui->ctrl_RGB_indicate_groupBox->setVisible(false);
  ui->ctrl_RGB_indicate_sleep_groupBox->setVisible(false);
  // 氛围灯灯效模式
  connect(ui->ctrl_RGB_mood_mode_switch_1, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_mood_mode_switch_2, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_mood_mode_switch_3, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);

  // 指示灯灯效模式
  connect(ui->ctrl_RGB_indicate_mode_switch_1, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_indicate_mode_switch_2, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_indicate_mode_switch_3, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);


  //休眠熄灯设置
  connect(ui->ctrl_rgb_checkBox_sleep_l_setting, &QCheckBox::stateChanged, this,
          &MainWindow::ctrlRGBEventSleepLSettingChanged);
  ctrlRGBTabChanged();
}

// 切换tab
void MainWindow::ctrlRGBTabChanged() {
  if (!checkCtrlConnect()) {
    return;
  }
  // 氛围灯
  getRgbStatusFunc(getCtrlConnectDev(), 0,
                   ui->ctrl_RGB_mood_mode_switch_1, ui->ctrl_RGB_mood_mode_switch_2, ui->ctrl_RGB_mood_mode_switch_3,
                   ui->ctrl_RGB_mood_horizontalSlider_color_l, ui->ctrl_RGB_mood_label_color_l_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_r, ui->ctrl_RGB_mood_label_color_r_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_g, ui->ctrl_RGB_mood_label_color_g_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_b, ui->ctrl_RGB_mood_label_color_b_value);
  // 指示灯
  int index = 0;
  if (ui->ctrl_RGB_indicat_lindex_mode_switch_1->isChecked()) {
    index = 1;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_2->isChecked()) {
    index = 1;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_3->isChecked()) {
    index = 2;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_4->isChecked()) {
    index = 3;
  } else {
    return;
  }
  getRgbStatusFunc(getCtrlConnectDev(), index,
                   ui->ctrl_RGB_indicate_mode_switch_1, ui->ctrl_RGB_indicate_mode_switch_2, ui->ctrl_RGB_indicate_mode_switch_3,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_l, ui->ctrl_RGB_indicate_label_color_l_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_r, ui->ctrl_RGB_indicate_label_color_r_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_g, ui->ctrl_RGB_indicate_label_color_g_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_b, ui->ctrl_RGB_indicate_label_color_b_value);

}

void MainWindow::ctrlRGBEventMoodModeSwitchClicked(bool checked) {
  if (!checked) { return; }
  auto clickedButton = qobject_cast<QRadioButton *>(QObject::sender());
  if (clickedButton == ui->ctrl_RGB_mood_mode_switch_1) {
    ui->ctrl_RGB_mood_groupBox->setVisible(false);
  } else if (clickedButton == ui->ctrl_RGB_mood_mode_switch_2) {
    ui->ctrl_RGB_mood_groupBox->setVisible(true);
  } else if (clickedButton == ui->ctrl_RGB_mood_mode_switch_3) {
    ui->ctrl_RGB_mood_groupBox->setVisible(true);
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_1) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(false);
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_2) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(true);
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_3) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(true);
  } else {
    return;
  }
}

void MainWindow::ctrlRGBEventSleepLSettingChanged(bool state) {
  if (state) {
    ui->ctrl_RGB_indicate_sleep_groupBox->setVisible(true);
  } else {
    ui->ctrl_RGB_indicate_sleep_groupBox->setVisible(false);
  }
}