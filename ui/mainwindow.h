#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "define.hpp"
#include "HID/devices.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    // ---扩展----
    std::shared_ptr<Lib::Image> m_userPushImage = std::make_shared<Lib::Image>();
    // 初始化
    void ctrlInit(QWidget *parent);
    // 选择设备
    void ctrlEventSwitchDevices( bool checked );
    // 选择图片
    void ctrlEventSwitchImage( bool checked );
    // 连接设备
    void ctrlEventConnectDevices();
    // 更新预览图像
    void ctrlEinkUpdateUserPreviewImage();
    // 更新滑块拖动事件
    void ctrlEventEinkValueChanged(int value);
    void ctrlEventEinkPushImage(bool checked);

    // ---扩展-墨水屏控制---
    // 初始化
    void ctrlEinkInit(QWidget *parent);
    // 选择图片
    void ctrlEinkEventSwitchImage(bool checked);

    Ui::MainWindow *ui;
    QPoint z;

    std::map<std::string, Lib::HWDevice> m_modelConnectStatus;
};
#endif // MAINWINDOW_H
