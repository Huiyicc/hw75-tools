//
// Created by 19254 on 2023/11/17.
//
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "knobaligndialog.h"
#include "ui/widget/MsgBox.hpp"

// 扩展appid
enum KnobAppID {
    APPID_VOLUME = 2,
    APPID_UPDOWN = 3,
    APPID_LIGHT = 4,
    APPID_WINDOW = 5,
    APPID_LEFTRIGHT = 6,
    APPID_HID = 7,
};

// 电机模式
enum KnobMode {
    // 关闭模式
    MODE_DISABLE = 0,
    // 惯性模式
    MODE_INERTIA,
    // 编码器模式
    MODE_ENCODER,
    // 荆轮编码器模式
    MODE_JINLUNENCODER,
    // 弹簧模式
    MODE_SPRING,
    // 阻尼模式
    MODE_DAMPED,
    // 拨杆模式
    MODE_PADDLE,
    // 旋转模式
    MODE_SPIN,
    // 智能模式
    MODE_INTELLIGENT,
};


std::map<int, int> g_appIDTable = {
    // 音量
    {0, APPID_VOLUME},
    // 滚动
    {1, APPID_UPDOWN},
    // 亮度
    {2, APPID_LIGHT},
    // 滑动
    {3, APPID_LEFTRIGHT},
};

// 初始化
void MainWindow::knobInit(QWidget *parent) {
  connect(ui->ctrl_knob_button_calibration, &QPushButton::clicked, this, &MainWindow::knobEventCalibration);
  connect(ui->ctrl_tabWidget_knob, &QTabWidget::currentChanged, this, &MainWindow::knobEventTabChanged);

  connect(ui->ctrl_knob_horizontalSlider_feedback, &QSlider::valueChanged, this,
          &MainWindow::knobEventSliderMoveFeedback);
  connect(ui->ctrl_knob_horizontalSlider_step, &QSlider::valueChanged, this, &MainWindow::knobEventSliderMoveFeedback);
  connect(ui->ctrl_knob_horizontalSlider_limit, &QSlider::valueChanged, this, &MainWindow::knobEventSliderMoveFeedback);

  connect(ui->ctrl_knob_mode_switch_1, &QRadioButton::clicked, this, &MainWindow::knobEventModeSwitchClicked);
  connect(ui->ctrl_knob_mode_switch_2, &QRadioButton::clicked, this, &MainWindow::knobEventModeSwitchClicked);
  connect(ui->ctrl_knob_mode_switch_3, &QRadioButton::clicked, this, &MainWindow::knobEventModeSwitchClicked);
  connect(ui->ctrl_knob_mode_switch_4, &QRadioButton::clicked, this, &MainWindow::knobEventModeSwitchClicked);


  knobEventTabChanged(0);

}

// 校准事件
void MainWindow::knobEventCalibration(bool checked) {
  auto pDialog = new KnobAlignDialog(this);
  pDialog->exec();
  delete pDialog;
}

// 选项卡切换事件
void MainWindow::knobEventTabChanged(int index) {
  if (!checkCtrlConnect()) {
    std::cout << "未连接设备" << std::endl;
    return;
  }
  if (g_appIDTable.find(index) == g_appIDTable.end()) {
    std::cout << "未知的应用" << std::endl;
    return;
  }
  auto devices = getCtrlConnectDev();
  Lib::HWDeviceTools tools;
  auto conf = tools.GetDynamicAppinConf(devices, g_appIDTable[index]);
  knobUpdataModeSwitchUI(conf);
}

// 更新模式切换UI
void MainWindow::knobUpdataModeSwitchUI(Lib::KnobAppConf &conf) {
  std::map<int, QRadioButton *> bMap = {
      {KnobMode::MODE_INTELLIGENT, ui->ctrl_knob_mode_switch_1},
      {KnobMode::MODE_ENCODER,     ui->ctrl_knob_mode_switch_2},
      {KnobMode::MODE_INERTIA,     ui->ctrl_knob_mode_switch_3},
      {KnobMode::MODE_DAMPED,      ui->ctrl_knob_mode_switch_4},
  };
  // 让所有按钮都不选中
  for (auto &i: bMap) {
    i.second->setChecked(false);
  }
  // 选中指定按钮
  if (bMap.find(conf.Mode) != bMap.end()) {
    bMap[conf.Mode]->setChecked(true);
  }
  // 力矩所有模式都有
  ui->ctrl_knob_horizontalSlider_feedback->setMaximum(conf.torqueLimitConf.max * 10);
  ui->ctrl_knob_horizontalSlider_feedback->setMinimum(conf.torqueLimitConf.min * 10);
  ui->ctrl_knob_horizontalSlider_feedback->setValue(conf.torqueLimitConf.value * 10);
  ui->ctrl_knob_label_feedback->setText(QString::number(conf.torqueLimitConf.value));
  // 有步数的模式
  if (conf.Mode == KnobMode::MODE_ENCODER
      || conf.Mode == KnobMode::MODE_INTELLIGENT) {
    ui->ctrl_knob_groupBox_step->setEnabled(true);
    ui->ctrl_knob_horizontalSlider_step->setMaximum(int(conf.stepConf.max));
    ui->ctrl_knob_horizontalSlider_step->setMinimum(int(conf.stepConf.min));
    ui->ctrl_knob_horizontalSlider_step->setValue(int(conf.stepConf.value));
    ui->ctrl_knob_label_step->setText(QString::number(conf.stepConf.value));
  } else {
    ui->ctrl_knob_groupBox_step->setEnabled(false);
  }
  // 智能模式
  if (conf.Mode == KnobMode::MODE_INTELLIGENT) {
    ui->ctrl_knob_groupBox_limit->setEnabled(true);
    ui->ctrl_knob_horizontalSlider_limit->setMaximum(conf.velocityLimitConf.max * 10);
    ui->ctrl_knob_horizontalSlider_limit->setMinimum(conf.velocityLimitConf.min * 10);
    ui->ctrl_knob_horizontalSlider_limit->setValue(conf.velocityLimitConf.value * 10);
    ui->ctrl_knob_label_limit->setText(QString::number(conf.velocityLimitConf.value));
  } else {
    ui->ctrl_knob_groupBox_limit->setEnabled(false);
  }

}

// 滑块拖动事件
void MainWindow::knobEventSliderMoveFeedback(int value) {
  auto movedSlider = qobject_cast<QSlider *>(QObject::sender());
  if (movedSlider == ui->ctrl_knob_horizontalSlider_feedback) {
    ui->ctrl_knob_label_feedback->setText(QString::number(value / 10.0));
  } else if (movedSlider == ui->ctrl_knob_horizontalSlider_step) {
    ui->ctrl_knob_label_step->setText(QString::number(value));
  } else if (movedSlider == ui->ctrl_knob_horizontalSlider_limit) {
    ui->ctrl_knob_label_limit->setText(QString::number(value / 10.0));
  }

}

// 模式切换按钮点击事件
void MainWindow::knobEventModeSwitchClicked(bool checked) {
  if (!checked) { return; }
  auto clickedButton = qobject_cast<QRadioButton *>(QObject::sender());
  std::map<QRadioButton *, int> bMap = {
      {ui->ctrl_knob_mode_switch_1, KnobMode::MODE_INTELLIGENT},
      {ui->ctrl_knob_mode_switch_2, KnobMode::MODE_ENCODER},
      {ui->ctrl_knob_mode_switch_3, KnobMode::MODE_INERTIA},
      {ui->ctrl_knob_mode_switch_4, KnobMode::MODE_DAMPED},
  };
  if (bMap.find(clickedButton) == bMap.end()) {
    std::cout << "未知的按钮" << std::endl;
    return;
  }
  auto devices = getCtrlConnectDev();
  Lib::KnobAppConf conf;
  conf.Mode = bMap[clickedButton];
  Lib::HWDeviceTools tools;
  try {
    tools.SetDynamicAppinConf(devices, g_appIDTable[ui->ctrl_tabWidget_knob->currentIndex()],
                              hid::msg::SetAppType::KnobMode,conf);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    MsgBox::critical(this, "错误", QString::fromStdString(e.what()));;
  }

}
