//
// Created by 19254 on 2023/9/27.
//
#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "utils/Log.hpp"
#include "widget/MsgBox.hpp"
#include <QFileDialog>
#include <QMessageBox>

bool rgbInit=false;

auto getRgbStatusFunc = [](const Lib::HWDevice &dev, int index,
                           QRadioButton *sw1, QRadioButton *sw2, QRadioButton *sw3,
                           QSlider *horizontal, QLabel *hValue,
                           QSlider *r, QLabel *rValue,
                           QSlider *g, QLabel *gValue,
                           QSlider *b, QLabel *bValue,
                           QCheckBox *slc, QSlider *sls, QLabel *slValue) {
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
      swp = sw1;
      swps[0] = sw2;
      swps[1] = sw3;
    } break;
    case 2: {
      swp = sw2;
      swps[0] = sw1;
      swps[1] = sw3;
    } break;
    case 3: {
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

  slc->setChecked(result.sleep_off);
  sls->setValue(result.sleep_brightness * 100.0f);
  slValue->setText(QString::number(result.sleep_brightness, 'f', 2));
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
  // 氛围灯灯效颜色亮度
  connect(ui->ctrl_RGB_mood_horizontalSlider_color_l, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);
  connect(ui->ctrl_RGB_mood_horizontalSlider_color_r, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);
  connect(ui->ctrl_RGB_mood_horizontalSlider_color_g, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);
  connect(ui->ctrl_RGB_mood_horizontalSlider_color_b, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);

  // 指示灯灯效模式
  connect(ui->ctrl_RGB_indicate_mode_switch_1, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_indicate_mode_switch_2, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  connect(ui->ctrl_RGB_indicate_mode_switch_3, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBEventMoodModeSwitchClicked);
  // 指示灯灯序号
  connect(ui->ctrl_RGB_indicate_lindex_mode_switch_1, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBIndicatIndexCheckedChanged);
  connect(ui->ctrl_RGB_indicate_lindex_mode_switch_2, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBIndicatIndexCheckedChanged);
  connect(ui->ctrl_RGB_indicate_lindex_mode_switch_3, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBIndicatIndexCheckedChanged);
  connect(ui->ctrl_RGB_indicate_lindex_mode_switch_4, &QPushButton::clicked, this,
          &MainWindow::ctrlRGBIndicatIndexCheckedChanged);
  // 指示灯灯效颜色亮度
  connect(ui->ctrl_RGB_indicate_horizontalSlider_color_l, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);
  connect(ui->ctrl_RGB_indicate_horizontalSlider_color_r, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);
  connect(ui->ctrl_RGB_indicate_horizontalSlider_color_g, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);


  //休眠熄灯设置
  connect(ui->ctrl_rgb_checkBox_sleep_l_setting, &QCheckBox::stateChanged, this,
          &MainWindow::ctrlRGBEventSleepLSettingChanged);
  connect(ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l, &QSlider::valueChanged, this,&MainWindow::ctrlEventRGBValueChanged);

  ctrlRGBTabChanged();
}

int MainWindow::ctrlRGBGetCurrentIndicateID() {
  if (ui->ctrl_RGB_indicate_lindex_mode_switch_1->isChecked()) {
    return 4;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_2->isChecked()) {
    return 1;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_3->isChecked()) {
    return 2;
  } else if (ui->ctrl_RGB_indicate_lindex_mode_switch_4->isChecked()) {
    return 3;
  }
  return 0;
}

// 切换tab
void MainWindow::ctrlRGBTabChanged() {
  if (!checkCtrlConnect()) {
    return;
  }
  rgbInit=false;
  // 氛围灯
  getRgbStatusFunc(getCtrlConnectDev(), 0,
                   ui->ctrl_RGB_mood_mode_switch_1, ui->ctrl_RGB_mood_mode_switch_2, ui->ctrl_RGB_mood_mode_switch_3,
                   ui->ctrl_RGB_mood_horizontalSlider_color_l, ui->ctrl_RGB_mood_label_color_l_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_r, ui->ctrl_RGB_mood_label_color_r_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_g, ui->ctrl_RGB_mood_label_color_g_value,
                   ui->ctrl_RGB_mood_horizontalSlider_color_b, ui->ctrl_RGB_mood_label_color_b_value,
                   ui->ctrl_rgb_checkBox_sleep_l_setting, ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l, ui->ctrl_RGB_indicate_sleep_label_light_l_value);
  // 指示灯
  int index = 0;
  if (ui->ctrl_RGB_indicate_lindex_mode_switch_1->isChecked()) {
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
                   ui->ctrl_RGB_indicate_horizontalSlider_color_b, ui->ctrl_RGB_indicate_label_color_b_value,
                   ui->ctrl_rgb_checkBox_sleep_l_setting, ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l, ui->ctrl_RGB_indicate_sleep_label_light_l_value);
  rgbInit=true;
}

void MainWindow::ctrlRGBEventMoodModeSwitchClicked(bool checked) {
  if (!checked) { return; }
  auto clickedButton = qobject_cast<QRadioButton *>(QObject::sender());
  int id = ctrlRGBGetCurrentIndicateID();
  int model = 0;
  if (clickedButton == ui->ctrl_RGB_mood_mode_switch_1) {
    ui->ctrl_RGB_mood_groupBox->setVisible(false);
    id = 0;
    model = 1;
  } else if (clickedButton == ui->ctrl_RGB_mood_mode_switch_2) {
    ui->ctrl_RGB_mood_groupBox->setVisible(true);
    id = 0;
    model = 2;
  } else if (clickedButton == ui->ctrl_RGB_mood_mode_switch_3) {
    ui->ctrl_RGB_mood_groupBox->setVisible(true);
    id = 0;
    model = 3;
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_1) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(false);
    model = 1;
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_2) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(true);
    model = 2;
  } else if (clickedButton == ui->ctrl_RGB_indicate_mode_switch_3) {
    ui->ctrl_RGB_indicate_groupBox->setVisible(true);
    model = 3;
  } else {
    return;
  }
  Lib::HWDeviceDynamicRGBStatus cfg;
  cfg.model = model;
  cfg.color_r = ui->ctrl_RGB_mood_horizontalSlider_color_r->value();
  cfg.color_g = ui->ctrl_RGB_mood_horizontalSlider_color_g->value();
  cfg.color_b = ui->ctrl_RGB_mood_horizontalSlider_color_b->value();
  cfg.brightness = ui->ctrl_RGB_mood_horizontalSlider_color_l->value() / 100.0f;
  cfg.sleep_off = !ui->ctrl_rgb_checkBox_sleep_l_setting->isChecked();
  cfg.sleep_brightness = ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l->value() / 100.0f;
  PrintInfo("SetDynamicRgbConfig send: {}", cfg.toJson().dump(2));
  //fmt::print("SetDynamicRgbConfig send: {}\n", cfg.toJson().dump(2));
  Lib::HWDeviceTools tools;
  tools.SetDynamicRgbConfig(getCtrlConnectDev(), id, cfg);
}

void MainWindow::ctrlRGBEventSleepLSettingChanged(bool state) {
  if (state) {
    ui->ctrl_RGB_indicate_sleep_groupBox->setVisible(true);
  } else {
    ui->ctrl_RGB_indicate_sleep_groupBox->setVisible(false);
  }
  int index = 0;
  Lib::HWDeviceDynamicRGBStatus cfg;
  cfg.model = ctrlRGBGetModel(1);
  cfg.color_r = ui->ctrl_RGB_mood_horizontalSlider_color_r->value();
  cfg.color_g = ui->ctrl_RGB_mood_horizontalSlider_color_g->value();
  cfg.color_b = ui->ctrl_RGB_mood_horizontalSlider_color_b->value();
  cfg.brightness = ui->ctrl_RGB_mood_horizontalSlider_color_l->value() / 100.0f;
  cfg.sleep_off = !ui->ctrl_rgb_checkBox_sleep_l_setting->isChecked();
  cfg.sleep_brightness = ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l->value() / 100.0f;
  PrintInfo("SetDynamicRgbConfig send id:{}cfg: {}",index, cfg.toJson().dump(2));
  Lib::HWDeviceTools tools;
  tools.SetDynamicRgbConfig(getCtrlConnectDev(), index, cfg);
}

void MainWindow::ctrlRGBIndicatIndexCheckedChanged(bool state) {
  int index = 0;
  auto clickedButton = qobject_cast<QRadioButton *>(QObject::sender());
  if (clickedButton == ui->ctrl_RGB_indicate_lindex_mode_switch_1) {
    index = 1;
  } else if (clickedButton == ui->ctrl_RGB_indicate_lindex_mode_switch_2) {
    index = 1;
  } else if (clickedButton == ui->ctrl_RGB_indicate_lindex_mode_switch_3) {
    index = 2;
  } else if (clickedButton == ui->ctrl_RGB_indicate_lindex_mode_switch_4) {
    index = 3;
  } else {
    return;
  }
  getRgbStatusFunc(getCtrlConnectDev(), index,
                   ui->ctrl_RGB_indicate_mode_switch_1, ui->ctrl_RGB_indicate_mode_switch_2, ui->ctrl_RGB_indicate_mode_switch_3,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_l, ui->ctrl_RGB_indicate_label_color_l_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_r, ui->ctrl_RGB_indicate_label_color_r_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_g, ui->ctrl_RGB_indicate_label_color_g_value,
                   ui->ctrl_RGB_indicate_horizontalSlider_color_b, ui->ctrl_RGB_indicate_label_color_b_value,
                   ui->ctrl_rgb_checkBox_sleep_l_setting, ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l, ui->ctrl_RGB_indicate_sleep_label_light_l_value);
}

int MainWindow::ctrlRGBGetModel(int type) {
  if (type == 1) {
    if (ui->ctrl_RGB_mood_mode_switch_1->isChecked()) {
      return 1;
    } else if (ui->ctrl_RGB_mood_mode_switch_2->isChecked()) {
      return 2;
    } else if (ui->ctrl_RGB_mood_mode_switch_3->isChecked()) {
      return 3;
    }
  } else if (type==2) {
    if (ui->ctrl_RGB_indicate_mode_switch_1->isChecked()) {
      return 1;
    } else if (ui->ctrl_RGB_indicate_mode_switch_2->isChecked()) {
      return 2;
    } else if (ui->ctrl_RGB_indicate_mode_switch_3->isChecked()) {
      return 3;
    }
  }
  return 0;
};

void MainWindow::ctrlEventRGBValueChanged(int value) {
  if (!rgbInit) {
    return;
  }
  std::map<QSlider*,QLabel*> sliderMap={
      {ui->ctrl_RGB_mood_horizontalSlider_color_l,ui->ctrl_RGB_mood_label_color_l_value},
      {ui->ctrl_RGB_mood_horizontalSlider_color_r,ui->ctrl_RGB_mood_label_color_r_value},
      {ui->ctrl_RGB_mood_horizontalSlider_color_g,ui->ctrl_RGB_mood_label_color_g_value},
      {ui->ctrl_RGB_mood_horizontalSlider_color_b,ui->ctrl_RGB_mood_label_color_b_value},
      {ui->ctrl_RGB_indicate_horizontalSlider_color_l,ui->ctrl_RGB_indicate_label_color_l_value},
      {ui->ctrl_RGB_indicate_horizontalSlider_color_r,ui->ctrl_RGB_indicate_label_color_r_value},
      {ui->ctrl_RGB_indicate_horizontalSlider_color_g,ui->ctrl_RGB_indicate_label_color_g_value},
      {ui->ctrl_RGB_indicate_horizontalSlider_color_b,ui->ctrl_RGB_indicate_label_color_b_value},
      {ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l,ui->ctrl_RGB_indicate_sleep_label_light_l_value},
  };
  int index = 0;
  Lib::HWDeviceDynamicRGBStatus cfg;
  auto eventObj = qobject_cast<QSlider *>(QObject::sender());
  if (eventObj == ui->ctrl_RGB_mood_horizontalSlider_color_l || eventObj == ui->ctrl_RGB_mood_horizontalSlider_color_r || eventObj == ui->ctrl_RGB_mood_horizontalSlider_color_g || eventObj == ui->ctrl_RGB_mood_horizontalSlider_color_b || eventObj == ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l) {
    index = 0;
    cfg.model = ctrlRGBGetModel(1);
    cfg.color_r = ui->ctrl_RGB_mood_horizontalSlider_color_r->value();
    cfg.color_g = ui->ctrl_RGB_mood_horizontalSlider_color_g->value();
    cfg.color_b = ui->ctrl_RGB_mood_horizontalSlider_color_b->value();
    cfg.brightness = ui->ctrl_RGB_mood_horizontalSlider_color_l->value() / 100.0f;
    cfg.sleep_off = !ui->ctrl_rgb_checkBox_sleep_l_setting->isChecked();
    cfg.sleep_brightness = ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l->value() / 100.0f;
  } else if (eventObj == ui->ctrl_RGB_indicate_horizontalSlider_color_l || eventObj == ui->ctrl_RGB_indicate_horizontalSlider_color_r || eventObj == ui->ctrl_RGB_indicate_horizontalSlider_color_g || eventObj == ui->ctrl_RGB_indicate_horizontalSlider_color_b) {
    index = ctrlRGBGetCurrentIndicateID();
    cfg.model = ctrlRGBGetModel(2);
    cfg.color_r = ui->ctrl_RGB_indicate_horizontalSlider_color_r->value();
    cfg.color_g = ui->ctrl_RGB_indicate_horizontalSlider_color_g->value();
    cfg.color_b = ui->ctrl_RGB_indicate_horizontalSlider_color_b->value();
    cfg.brightness = ui->ctrl_RGB_indicate_horizontalSlider_color_l->value() / 100.0f;
    cfg.sleep_off = !ui->ctrl_rgb_checkBox_sleep_l_setting->isChecked();
    cfg.sleep_brightness = ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l->value() / 100.0f;
  } else {
    return;
  }
  if (eventObj!=ui->ctrl_RGB_mood_horizontalSlider_color_l && eventObj!=ui->ctrl_RGB_indicate_horizontalSlider_color_l && eventObj!=ui->ctrl_RGB_indicate_sleep_horizontalSlider_light_l) {
    sliderMap[eventObj]->setText(QString::number(value));
  } else {
    sliderMap[eventObj]->setText(QString::number(value / 100.0f, 'f', 2));
  }
  PrintInfo("SetDynamicRgbConfig send id:{}cfg: {}",index, cfg.toJson().dump(2));
  Lib::HWDeviceTools tools;
  tools.SetDynamicRgbConfig(getCtrlConnectDev(), index, cfg);

};