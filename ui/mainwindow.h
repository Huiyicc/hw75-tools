#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QHeaderView>
#include "define.hpp"
#include "HID/devices.hpp"
#include "QTimer"

struct PluginUI;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void sysButtonEventMin(bool checked);

    void initMenu();

    bool ctrlEinkPushImage(const char *data, int len);

private:

    bool checkCtrlConnect();

    Lib::HWDevice getCtrlConnectDev();

    // ---扩展----
    std::shared_ptr<Lib::Image> m_userPushImage = std::make_shared<Lib::Image>();

    // 初始化
    void ctrlInit(QWidget *parent);

    // 选择设备
    void ctrlEventSwitchDevices(bool checked);

    // 连接设备
    void ctrlEventConnectDevices();

    // ---扩展-电机模块---
    void knobInit(QWidget *parent);

    void knobEventCalibration(bool checked);

    // 切换app选项卡
    void knobEventTabChanged(int);

    void knobUpdataModeSwitchUI(Lib::KnobAppConf&);
    // 参数条拖动事件
    void knobEventSliderMoveFeedback(int);
    // 电机模式切换事件
    void knobEventModeSwitchClicked(bool);

    // ---扩展-墨水屏控制---
    // 初始化
    void ctrlEinkInit(QWidget *parent);

    // 选择图片
    void ctrlEventEinkSwitchImage(bool checked);

    // 更新预览图像
    void ctrlEinkUpdateUserPreviewImage();

    // 更新滑块拖动事件
    void ctrlEventEinkValueChanged(int value);

    // 更新屏幕图片按钮事件
    void ctrlEventEinkPushImage(bool checked);


    // ---扩展-OLED控制---
    // 初始化
    void ctrlOLEDInit(QWidget *parent);

    // 选择图片
    void ctrlEventOLEDSwitchImage(bool checked);

    // 更新滑块拖动事件
    void ctrlEventOLEDValueChanged(int value);

    // 更新屏幕图片按钮事件
    void ctrlEventOLEDPushImage(bool checked);

    // 更新预览图像
    void ctrlOLEDUpdateUserPreviewImage();

    // --扩展-插件---
    // 初始化插件列表
    void ctrlPluginInit(QWidget *parent);

    // 插件列表点击事件
    void ctrlEventPluginListClicked(QModelIndex index);

    // 显示插件UI
    void ctrlPluginUIShow(QString &name);

    // 解析插件UI
    void
    ctrlPluginParseUI(nlohmann::json &config, QWidget *, PluginUI *pData = nullptr, bool father = false, int top = 0);

    // 保存插件配置
    void ctrlPluginSaveConfig(bool checked);

    // 插件定时事件
    void ctrlPluginTickEvent();

    // 插件提交事件
    void ctrlPluginSubmint(bool checked);

    // 扩展-插件-右键菜单-弹出
    void ctrlPluginEjectMenuEvent(const QPoint &pos);

    // 扩展-插件-右键菜单
    void ctrlPluginContextMenuEvent();


private:
    Ui::MainWindow *ui;
    QPoint z;

    std::map<std::string, Lib::HWDevice> m_modelConnectStatus;

    QSystemTrayIcon m_sysTrayIcon;
    QMenu m_trayMenu;

    QMenu *m_pCtrlPluginContextMenu = nullptr;

    std::shared_ptr<QTimer> m_pluginTick;
};

#endif // MAINWINDOW_H
