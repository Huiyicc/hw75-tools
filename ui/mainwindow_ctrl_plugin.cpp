//
// Created by 19254 on 2023/10/31.
//
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "plugin/Plugin.hpp"
#include "QThread"
#include "QScrollArea"
#include "utils/config.hpp"
#include "ui/widget/MsgBox.hpp"
#include <cmath>
#include <QMessageBox>
#include <QFormLayout>
#include <QCheckBox>


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
	ui->ctrl_plugin_tableWidget_pluginlist->setHorizontalHeaderLabels(QStringList() << "启用状态" << "插件名");
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
		auto cfg = GetConfigData();
		auto cfgIter = cfg.Ctrl.Plugins.find(plugin.second.RawName.toStdString());
		if (cfgIter != cfg.Ctrl.Plugins.end()) {
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

void MainWindow::ctrlPluginParseUI(nlohmann::json &config, QWidget *widget, PluginUI *pData, bool father, int top) {
	if (father) {
		if (!config["widgets"].is_array()) {
			return;
		}
		// 父分组
		int lTop = 0;
		for (auto &widgetConfig: config["widgets"]) {
			ctrlPluginParseUI(widgetConfig, widget, pData, false, lTop);
			lTop += 30;
		}
		lTop += (20 + 5);
		double maxWidgetWidth = 700 - 20;
		widget->setGeometry(0, 0, maxWidgetWidth, lTop);
		widget->setVisible(false);
		return;
	}
	if (!config.is_array()) {
		return;
	}
	// 子分组
	int x = 10;
	double maxWidgetWidth = 700 - 30;
	for (auto &widgetConfig: config) {
		if (!widgetConfig["type"].is_string()
		    || !widgetConfig["text"].is_string()
		    || !widgetConfig["layout"].is_number()
		    || !widgetConfig["bind"].is_string()
				) {
			std::cout << "faild:" << widgetConfig << std::endl;
			continue;
		}

		auto layout = widgetConfig["layout"].get<double>();
		auto widgetWidth = round(maxWidgetWidth * layout / 10);
		auto type = widgetConfig["type"].get<std::string>();
		auto bind = widgetConfig["bind"].get<std::string>();
		if (type == "text") {
			// 文本
			auto label = new QLabel(widget);
			label->setGeometry(x, top, widgetWidth, 20);
			label->setText(widgetConfig["text"].get<std::string>().c_str());
			label->setStyleSheet("background:rgb(29, 37, 86);\n"
			                     "color:rgb(138, 144, 176);");
			label->show();
		} else if (type == "input") {
			// 输入框
			auto input = new QLineEdit(widget);
			input->setGeometry(x, top, widgetWidth, 20);
			input->setText(widgetConfig["text"].get<std::string>().c_str());
			input->setStyleSheet("background:rgba(75, 80, 120,1);\n"
			                     "color: rgb(211, 216, 237);\n"
			                     "border-radius: 3px;\n"
			                     "border: none;\n"
			                     "padding: 5px 5px 5px 5px;");
			input->show();
			pData->Bind[bind.c_str()] = PluginUI::BindInfo{
					.Widget = input,
					.BindName = bind.c_str(),
					.WidgetType=WIDGET_INPUT
			};
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
			button->setGeometry(x, top, widgetWidth, 20);
			button->setText(widgetConfig["text"].get<std::string>().c_str());
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
					.WidgetType=WIDGET_BUTTON
			};
			connect(button, &QPushButton::clicked, this, &MainWindow::ctrlPluginSubmint);

		} else if (type == "checkbox") {
			// 复选框
			auto wigdet = new QCheckBox(widget);
			wigdet->setGeometry(x, top, widgetWidth, 20);
			wigdet->setText(widgetConfig["text"].get<std::string>().c_str());
			wigdet->setStyleSheet("background:rgb(29, 37, 86); color:rgb(138, 144, 176);");
			wigdet->show();
			pData->Bind[bind.c_str()] = PluginUI::BindInfo{
					.Widget = wigdet,
					.BindName = bind.c_str(),
					.WidgetType=WIDGET_CHECKBOX,
			};
			//connect(button, &QPushButton::clicked, this, &MainWindow::ctrlPluginSubmint);

		} else {
			std::cout << "未知的控件类型:" << type << std::endl;
			continue;
		}

		x += widgetWidth;

	}
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
	std::cout << "加载插件UI:" << pluginName.toStdString() << std::endl;
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
	std::cout << "插件Tick事件" << std::endl;
	auto plugins = Lib::Plugin::GetPlugins("ctrl");
	for (auto &plugin: plugins) {
		if (plugin.second.Enable) {
			try {
				if (!Lib::Plugin::CallPluginTimedEvent("ctrl", plugin.second.RawName)) {
					std::cout << "插件Tick事件失败:" << plugin.second.RawName.toStdString() << std::endl;
					std::cout << "LastError:" << Lib::Plugin::CallPluginGetLastError("ctrl", plugin.second.RawName)
					          << std::endl;
				}
			} catch (std::exception &e) {
				std::cout << "插件Tick事件异常:" << e.what() << std::endl;
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
	if (!Lib::Plugin::CallPluginSubmit("ctrl", ctrl_plugin_status, to_string(resultConfig).c_str())) {
		MsgBox::critical(this, "错误", "插件提交事件失败\n" +
		                               QString::fromStdString(
				                               Lib::Plugin::CallPluginGetLastError("ctrl", ctrl_plugin_status)));
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
	cfg->getConfig().Ctrl.Plugins[pluginInfo.RawName.toStdString()].Enable = pluginInfo.Enable;
	cfg->saveConfig();
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
