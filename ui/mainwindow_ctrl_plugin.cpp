//
// Created by 19254 on 2023/10/31.
//
#include "./ui_mainwindow.h"
#include "QScrollArea"
#include "QThread"
#include "mainwindow.h"
#include "plugin/Plugin.hpp"
#include "ui/widget/MsgBox.hpp"
#include "utils/Defer.hpp"
#include "utils/Log.hpp"
#include "utils/config.hpp"
#include <QCheckBox>
#include <QTextEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <cmath>

// 插件UI类型
enum PluginWidgetType {
  // 标签文本组件
  WIDGET_TEXT,
  // 输入框组件
  WIDGET_INPUT,
  // 分割线
  WIDGET_DIVIDER,
  // 提交按钮
  WIDGET_BUTTON,
  // 复选框
  WIDGET_CHECKBOX,
  // 多行输入框
  WIDGET_INPUT_ML
};

// 插件UI
struct PluginUI {
  // 插件容器
  QWidget *GroupWidget;
  struct BindInfo {
    // 绑定的控件
    QWidget *Widget;
    // 绑定的映射名称
    QString BindName;
    // 绑定的控件类型
    PluginWidgetType WidgetType;
  };
  std::map<QString, BindInfo> Bind;
  std::map<void *, BindInfo> SubmitBind;
};

// 当前ui
QString ctrl_plugin_status = "";
// 当前配置区的ui
std::map<QString, PluginUI> ctrl_plugin_ui;
bool stopBack = false;

std::shared_ptr<std::thread> m_pluginCallBack = std::make_shared<std::thread>([]() {
  auto initDevPathFunc = []() -> std::string {
    std::string rePath;

    // 获取连接的HID设备列表
    auto devs = hid_enumerate(Lib::HWDeviceTools::HWVID, Lib::HWDeviceTools::HWPID);
    auto cur_dev = devs;

    // 开始遍历设备列表
    while (cur_dev) {
      // 判断设备是否为USB HID设备

      if (cur_dev->product_id == Lib::HWDeviceTools::HWPID && cur_dev->vendor_id == Lib::HWDeviceTools::HWVID// 判断是否为瀚文设备
          && cur_dev->usage_page == Lib::HWDeviceTools::USB_USAGE_PAGE && cur_dev->usage == Lib::HWDeviceTools::USB_USAGE_PLUGIN) {
        // 打开设备
        auto handle = hid_open_path(cur_dev->path);
        rePath = cur_dev->path;
        hid_close(handle);
      }
      // 指向下一个设备
      cur_dev = cur_dev->next;
    }
    // 释放设备列表
    hid_free_enumeration(devs);
    return rePath;
  };
  std::string devPath;
  while (!stopBack) {
    DEFER(QThread::msleep(1000));
    //    if (lMainWind == nullptr) {
    //      PrintDebug("插件监听线程: 主窗口未初始化");
    //      continue;
    //    }
    if (devPath.empty()) {
      devPath = initDevPathFunc();
      if (devPath.empty()) {
        QThread::msleep(5000);
        continue;
      }
    }
    try {
      auto devHandle = hid_open_path(devPath.c_str());
      DEFER(hid_close(devHandle));
      if (devHandle == nullptr) {
        devPath = "";
        auto errMsg = QString::fromStdWString(hid_error(NULL));
        throw std::runtime_error(errMsg.toStdString());
      }
      std::vector<unsigned char> buf(66, 0);
      auto r = hid_read(devHandle, buf.data(), 66);
      if (r <= 0) {
        auto errMsg = QString::fromStdWString(hid_error(devHandle));
        throw std::runtime_error(errMsg.toStdString());
      }
      PrintDebug("插件监听线程: 读取到数据");
      auto dataPtr = buf.data();
      //第一个字节为报告id,第二个字节为完整性,第三个字节为数据长度
      if (dataPtr[0] != 0x6) {
        throw std::runtime_error("报告ID错误");
      }
      if (dataPtr[1] != 0x0) {
        throw std::runtime_error("数据完整性错误");
      }
      int lents = dataPtr[2];
      if (lents > Lib::HWDeviceTools::HID_REPORT_COUNT) {
        throw std::runtime_error("data lens is to long.");
      }
      // 根据长度写入流
      google::protobuf::io::ArrayInputStream arrayInput((void *) (&dataPtr[2]), lents);
      // 解码字节流
      hid::msg::CtrlPluginMessage message;
      Lib::HWDeviceTools::readDelimitedD2P(&arrayInput, &message);
      if (message.has_button_status()) {
        // 按钮消息
        auto buttonStatus = message.button_status();
        Lib::Plugin::CallPluginButtonPin("ctrl", Lib::Plugin::ButtonPinCallType(buttonStatus.status()));
      }


    } catch (std::exception &e) {
      PrintError("插件监听线程异常: {}", e.what());
    }
  }
});

void MainWindow::ctrlPluginInit(QWidget *parent) {
  connect(ui->ctrl_plugin_button_saveconfig, &QPushButton::clicked, this, &MainWindow::ctrlPluginSaveConfig);
  // 首先加载所有插件
  Lib::Plugin::PluginsInitAll("ctrl");
  connect(ui->ctrl_plugin_tableWidget_pluginlist, &QTableWidget::clicked, this,
          &MainWindow::ctrlEventPluginListClicked);
  /// 右键菜单
  //m_pCtrlPluginContextMenu = new QMenu(ui->ctrl_plugin_tableWidget_pluginlist);
  m_pCtrlPluginContextMenu = new QMenu(this);
  m_pCtrlPluginContextMenu->setStyleSheet("QMenu{"
                                          "color:rgb(211, 216, 237);"
                                          "}"
                                          "QMenu::item:selected{\n"
                                          "background-color: rgba(75, 80, 121,0.8);\n"
                                          "}");
  //	connect(m_pActionDel, &QMenu::triggered, this,
  //	        &MainWindow::ctrlPluginContextMenuEvent);
  ui->ctrl_plugin_tableWidget_pluginlist->verticalHeader()->setVisible(false);
  ui->ctrl_plugin_tableWidget_pluginlist->setColumnCount(3);
  ui->ctrl_plugin_tableWidget_pluginlist->setColumnHidden(2, true);
  ui->ctrl_plugin_tableWidget_pluginlist->setHorizontalHeaderLabels(QStringList() << "启用状态"
                                                                                  << "插件名");
  ui->ctrl_plugin_tableWidget_pluginlist->setColumnWidth(0, 80);
  ui->ctrl_plugin_tableWidget_pluginlist->setColumnWidth(1, ui->ctrl_plugin_tableWidget_pluginlist->width() - 82);
  ui->ctrl_plugin_tableWidget_pluginlist->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->ctrl_plugin_tableWidget_pluginlist->setContextMenuPolicy(Qt::CustomContextMenu);
  ui->ctrl_plugin_tableWidget_pluginlist->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->ctrl_plugin_tableWidget_pluginlist->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->ctrl_plugin_tableWidget_pluginlist->setStyleSheet("QTableWidget {\n"
                                                        "background:rgb(29, 37, 86);\n"
                                                        "outline: none;"
                                                        "}\n"
                                                        "QHeaderView::section {\n"
                                                        "background-color: rgb(29, 37, 86);\n"
                                                        "background:rgb(29, 37, 86);\n"
                                                        "border-left: 1px solid rgb(255,255,255);\n"
                                                        "border-radius: 0px;\n"
                                                        "color: white;\n"
                                                        "}\n"
                                                        "QTableWidget::item {\n"
                                                        "color: white;\n"
                                                        "}\n"
                                                        "QTableWidget::item:selected {"
                                                        "color: rgb(211, 216, 237);"
                                                        "background-color: rgb(75, 80, 121); "
                                                        "}"
                                                        "QTableWidget::item { "
                                                        "color: rgb(211, 216, 237);"
                                                        "}");

  connect(ui->ctrl_plugin_tableWidget_pluginlist, &QTableWidget::customContextMenuRequested,
          this, &MainWindow::ctrlPluginEjectMenuEvent);


  auto &plugins = Lib::Plugin::GetPlugins("ctrl");
  // 读取/初始化所有插件
  for (auto &plugin: plugins) {
    auto cfg = GetConfigInstance();
    auto cfgIter = cfg->Ctrl.Plugins.find(plugin.second.RawName.toStdString());
    if (cfgIter != cfg->Ctrl.Plugins.end()) {
      plugin.second.Enable = cfgIter->second.Enable;
      if (plugin.second.Enable) {
        Lib::Plugin::CallPluginInit("ctrl", plugin.second.RawName, plugin.second);
      }
    }
    ui->ctrl_plugin_tableWidget_pluginlist->insertRow(ui->ctrl_plugin_tableWidget_pluginlist->rowCount());
    auto item = new QTableWidgetItem(plugin.second.Enable ? "√" : "×");
    item->setTextAlignment(Qt::AlignCenter);
    ui->ctrl_plugin_tableWidget_pluginlist->setItem(ui->ctrl_plugin_tableWidget_pluginlist->rowCount() - 1, 0,
                                                    item);
    ui->ctrl_plugin_tableWidget_pluginlist->setItem(ui->ctrl_plugin_tableWidget_pluginlist->rowCount() - 1, 1,
                                                    new QTableWidgetItem(plugin.second.Name));
    ui->ctrl_plugin_tableWidget_pluginlist->setItem(ui->ctrl_plugin_tableWidget_pluginlist->rowCount() - 1, 2,
                                                    new QTableWidgetItem(plugin.second.RawName));
  }
  ui->ctrl_plugin_scrollArea_pluginconfigview->setWidgetResizable(true);
  m_pluginTick = std::make_shared<QTimer>(this);
  connect(m_pluginTick.get(), &QTimer::timeout, this, &MainWindow::ctrlPluginTickEvent);
  m_pluginTick->start(10000);
  // lMainWind = this;
}

void MainWindow::ctrlPluginUnInit() {
  stopBack = true;
  m_pluginCallBack->detach();
  m_pluginCallBack.reset();
  m_pluginCallBack = nullptr;
}

void MainWindow::ctrlPluginUIShow(QString &name) {
  if (ctrl_plugin_status == name) {
    return;
  }
  if (ctrl_plugin_ui.find(name) == ctrl_plugin_ui.end()) {
    return;
  }
  if (!ctrl_plugin_status.isEmpty()) {
    ctrl_plugin_ui[ctrl_plugin_status].GroupWidget->setVisible(false);
  }
  ctrl_plugin_ui[name].GroupWidget->setVisible(true);
  ctrl_plugin_status = name;
  auto plugins = Lib::Plugin::GetPlugins("ctrl");
  if (plugins.find(name) == plugins.end()) {
    return;
  }
  auto plugin = plugins[name];
  QString infoStr = "插件名称: " + plugin.Name + "\n" +
                    "插件版本: " + plugin.Version + "\n" +
                    "插件作者: " + plugin.Author + "\n" +
                    "插件描述: " + plugin.Description + "\n";

  ui->ctrl_plugin_label_info->setText(infoStr);
}

float MainWindow::ctrlPluginParseUI(nlohmann::json &config, QWidget *widget, PluginUI *pData, bool father, int top) {
  if (father) {
    if (!config["widgets"].is_array()) {
      return 30;
    }
    // 父分组
    int lTop = 0;
    for (auto &widgetConfig: config["widgets"]) {
      lTop += ctrlPluginParseUI(widgetConfig, widget, pData, false, lTop);
    }
    lTop += (20 + 5);
    double maxWidgetWidth = 700 - 20;
    widget->setGeometry(0, 0, maxWidgetWidth, lTop);
    widget->setVisible(false);
    return 30;
  }
  if (!config.is_array()) {
    return 30;
  }
  // 子分组
  int x = 10;
  double maxWidgetWidth = 700 - 30;
  float maxH = 20;
  for (auto &widgetConfig: config) {
    if (!widgetConfig["type"].is_string() || !widgetConfig["text"].is_string() || !widgetConfig["layout"].is_number() || !widgetConfig["bind"].is_string()) {
      PrintInfo("注册插件UI失败: {}", widgetConfig.dump());
      continue;
    }

    auto layout = widgetConfig.value<double>("layout", 8);
    auto widgetWidth = round(maxWidgetWidth * layout / 10);
    auto type = widgetConfig.value<std::string>("type", "");
    auto bind = widgetConfig.value<std::string>("bind", "");
    auto height = widgetConfig.value<float>("height", 20);
    height= height==0?20:height;
    maxH = std::max(maxH, height);
    if (type == "text") {
      // 文本
      auto label = new QLabel(widget);
      label->setGeometry(x, top, widgetWidth, height);
      label->setText(widgetConfig.value<std::string>("text","").c_str());
      label->setStyleSheet("background:rgb(29, 37, 86);\n"
                           "color:rgb(138, 144, 176);");
      label->show();
    } else if (type == "input") {
      // 输入框
      auto input = new QLineEdit(widget);
      input->setGeometry(x, top, widgetWidth, height);
      input->setText(widgetConfig.value<std::string>("text","").c_str());
      input->setStyleSheet("background:rgba(75, 80, 120,1);\n"
                           "color: rgb(211, 216, 237);\n"
                           "border-radius: 3px;\n"
                           "border: none;\n"
                           "padding: 5px 5px 5px 5px;");
      input->show();
      pData->Bind[bind.c_str()] = PluginUI::BindInfo{
          .Widget = input,
          .BindName = bind.c_str(),
          .WidgetType = WIDGET_INPUT};
    } else if (type == "divider") {
      // 分割线
      // 布局不适用于分割线
      auto divider = new QLabel(widget);
      divider->setGeometry(x, top + 9, maxWidgetWidth, 1);
      divider->setStyleSheet("background-color: rgb(75, 80, 121);");
      divider->show();
    } else if (type == "submit") {
      // 提交按钮
      auto button = new QPushButton(widget);
      button->setGeometry(x, top, widgetWidth, height);
      button->setText(widgetConfig.value<std::string>("text","提交").c_str());
      button->setStyleSheet("QPushButton{\n"
                            "background:qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:0 #3fd38f, stop:1 #50b4cb);\n"
                            "color: rgb(211, 216, 237);\n"
                            "border-radius: 10px;\n"
                            "border: none;\n"
                            "}\n"
                            "QPushButton:focus {\n"
                            "    outline: none;\n"
                            "}");
      button->show();
      pData->SubmitBind[button] = PluginUI::BindInfo{
          .Widget = button,
          .BindName = bind.c_str(),
          .WidgetType = WIDGET_BUTTON};
      connect(button, &QPushButton::clicked, this, &MainWindow::ctrlPluginSubmint);

    } else if (type == "checkbox") {
      // 复选框
      auto wigdet = new QCheckBox(widget);
      wigdet->setGeometry(x, top, widgetWidth, height);
      wigdet->setText(widgetConfig.value<std::string>("text","").c_str());
      wigdet->setStyleSheet("background:rgb(29, 37, 86); color:rgb(138, 144, 176);");
      wigdet->show();
      pData->Bind[bind.c_str()] = PluginUI::BindInfo{
          .Widget = wigdet,
          .BindName = bind.c_str(),
          .WidgetType = WIDGET_CHECKBOX,
      };
      //connect(button, &QPushButton::clicked, this, &MainWindow::ctrlPluginSubmint);

    } else if (type == "input_ml") {
      // 多行输入框
      auto wigdet = new QTextEdit(widget);
      wigdet->setGeometry(x, top, widgetWidth, height);
      wigdet->setText(widgetConfig.value<std::string>("text","").c_str());
      wigdet->setStyleSheet("background:rgba(75, 80, 120,1);\n"
                            "color: rgb(211, 216, 237);\n"
                            "border-radius: 3px;\n"
                            "border: none;\n"
                            "padding: 5px 5px 5px 5px;");
      wigdet->show();
      pData->Bind[bind.c_str()] = PluginUI::BindInfo{
          .Widget = wigdet,
          .BindName = bind.c_str(),
          .WidgetType = WIDGET_INPUT_ML};

    } else {
      PrintInfo("未知的控件类型: {}", type);
      continue;
    }

    x += widgetWidth;
  }
  return maxH + 10;
}

void MainWindow::ctrlEventPluginListClicked(QModelIndex index) {
  // 获取当行数据
  auto row = index.row();
  auto item = ui->ctrl_plugin_tableWidget_pluginlist->item(row, 2);
  if (item == nullptr) {
    return;
  }
  auto pluginName = item->text();
  if (ctrl_plugin_status == pluginName || pluginName.isEmpty()) {
    return;
  }
  if (ctrl_plugin_ui.find(pluginName) != ctrl_plugin_ui.end()) {
    // 已经加载过了
    ctrlPluginUIShow(pluginName);
    return;
  }
  PrintDebug("加载插件UI: {}", pluginName.toStdString());
  // 申请插件布局
  auto uiRaw = Lib::Plugin::CallPluginConfigUI("ctrl", pluginName);
  auto uiData = nlohmann::json::parse(uiRaw);
  // 插件容器
  auto area = new QScrollArea(ui->ctrl_plugin_groupBox_fater);
  area->setGeometry(10, 32, 701, 261);
  area->setStyleSheet("border: none;\n"
                      "background:rgb(29, 37, 86);\n"
                      "color: rgb(211, 216, 237);\n"
                      "border-radius: 12px;");
  area->show();
  QWidget *pWgt = new QWidget;

  // 配置区域缓存
  ctrl_plugin_ui[pluginName] = PluginUI{};
  ctrl_plugin_ui[pluginName].GroupWidget = area;
  ctrlPluginParseUI(uiData, pWgt, &ctrl_plugin_ui[pluginName], true, 0);

  area->setWidget(pWgt);

  ctrlPluginUIShow(pluginName);
}

nlohmann::json GetPluginConfigJsonStr() {
  if (ctrl_plugin_status.isEmpty()) {
    return "";
  }
  auto iter = ctrl_plugin_ui.find(ctrl_plugin_status);
  if (iter == ctrl_plugin_ui.end()) {
    return "";
  }
  auto result = nlohmann::json::object();
  for (auto &bind: iter->second.Bind) {
    if (bind.second.WidgetType == WIDGET_INPUT) {
      auto input = (QLineEdit *) bind.second.Widget;
      result[bind.first.toStdString()] = input->text().toStdString();
    } else if (bind.second.WidgetType == WIDGET_INPUT_ML) {
      auto input = (QTextEdit *) bind.second.Widget;
      result[bind.first.toStdString()] = input->toPlainText().toStdString();
    } else if (bind.second.WidgetType == WIDGET_CHECKBOX) {
      auto input = (QCheckBox *) bind.second.Widget;
      result[bind.first.toStdString()] = input->isChecked();
    }
  }
  return result;
}

void MainWindow::ctrlPluginSaveConfig(bool checked) {
  auto result = GetPluginConfigJsonStr();
  if (result.empty()) {
    return;
  }
  if (Lib::Plugin::CallPluginSaveConfig("ctrl", ctrl_plugin_status, to_string(result).c_str())) {
    MsgBox::information(this, "成功", "保存配置成功");
  } else {
    MsgBox::critical(this, "错误", "保存配置失败");
  };
}

void MainWindow::ctrlPluginTickEvent() {
  // 插件Tick事件,10秒一次
  PrintInfo("插件Tick事件");
  auto plugins = Lib::Plugin::GetPlugins("ctrl");
  for (auto &plugin: plugins) {
    if (plugin.second.Enable) {
      try {
        if (!Lib::Plugin::CallPluginTimedEvent("ctrl", plugin.second.RawName)) {
          PrintError("插件Tick事件失败: {}", plugin.second.RawName.toStdString());
          PrintError("LastError: {}", Lib::Plugin::CallPluginGetLastError("ctrl", plugin.second.RawName));
        }
      } catch (std::exception &e) {
        PrintError("插件Tick事件异常: {}", e.what());
      }
    }
  }
}

void MainWindow::ctrlPluginSubmint(bool checked) {
  auto clickedButton = qobject_cast<QPushButton *>(sender());
  if (clickedButton == nullptr) {
    return;
  }
  auto info = ctrl_plugin_ui[ctrl_plugin_status].SubmitBind.find((void *) clickedButton);
  if (info == ctrl_plugin_ui[ctrl_plugin_status].SubmitBind.end()) {
    return;
  }
  auto &pluginInfo = Lib::Plugin::GetPlugin("ctrl", ctrl_plugin_status);
  if (!pluginInfo.Enable) {
    MsgBox::critical(this, "错误", "插件未启用");
    return;
  }
  auto result = nlohmann::json::object();
  result["event_bind"] = info->second.BindName.toStdString();
  auto resultConfig = GetPluginConfigJsonStr();
  if (resultConfig.empty()) {
    return;
  }
  result["config"] = resultConfig;
  if (!Lib::Plugin::CallPluginSubmit("ctrl", ctrl_plugin_status, to_string(result).c_str())) {
    MsgBox::critical(this, "错误", "插件提交事件失败\n" + QString::fromStdString(Lib::Plugin::CallPluginGetLastError("ctrl", ctrl_plugin_status)));
    return;
  }
  MsgBox::information(this, "成功", "插件提交事件成功");
}

void MainWindow::ctrlPluginContextMenuEvent() {
  auto selectedIndexes = ui->ctrl_plugin_tableWidget_pluginlist->selectionModel()->selectedIndexes();
  if (selectedIndexes.isEmpty()) {
    return;
  }

  // 检查启用
  auto &pluginInfo = Lib::Plugin::GetPlugin("ctrl", selectedIndexes[2].data().toString());
  pluginInfo.Enable = !pluginInfo.Enable;
  if (pluginInfo.Enable) {
    Lib::Plugin::CallPluginInit("ctrl", pluginInfo.RawName, pluginInfo);
  } else {
    Lib::Plugin::CallPluginUnRegister("ctrl", pluginInfo.RawName);
  }
  auto cfg = GetConfigInstance();
  cfg->Ctrl.Plugins[pluginInfo.RawName.toStdString()].Enable = pluginInfo.Enable;
  cfg.saveConfig();
  // 更新表格
  ui->ctrl_plugin_tableWidget_pluginlist->item(selectedIndexes[0].row(), 0)->setText(pluginInfo.Enable ? "√" : "×");
}

void MainWindow::ctrlPluginEjectMenuEvent(const QPoint &pos) {
  // 获取选中的行索引
  auto selectedIndexes = ui->ctrl_plugin_tableWidget_pluginlist->selectionModel()->selectedIndexes();
  if (selectedIndexes.isEmpty()) {
    return;
  }
  // 清空之前的菜单项（如果有）
  m_pCtrlPluginContextMenu->clear();
  // 检查启用
  auto &pluginInfo = Lib::Plugin::GetPlugin("ctrl", selectedIndexes[2].data().toString());

  // 创建并添加新的菜单项
  QAction *action = new QAction(pluginInfo.Enable ? "禁用" : "启用", this);
  connect(action, &QAction::triggered, this, &MainWindow::ctrlPluginContextMenuEvent);
  m_pCtrlPluginContextMenu->addAction(action);
  // 在鼠标位置显示
  m_pCtrlPluginContextMenu->exec(QCursor::pos());
}
