//
// Created by 19254 on 2023/11/17.
//
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/widget/MsgBox.hpp"
#include "knobaligndialog.h"

#include <QFileDialog>
#include <QThread>
#include <thread>
#include <QRandomGenerator>
#include <QStack>

#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_plot_marker.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_curve_fitter.h>
#include <Qwt/qwt_spline_curve_fitter.h>

#include "fmt/format.h"
#include "utils/math.hpp"

struct sChart {
    QwtPlotCurve *curve = nullptr;
    //QStack<QPointF> points;
    QVector<QPointF> points;
};

struct sCtrlChart {
    QwtPlot *Plot = nullptr;
    // 当前角度
    sChart currentAngle;
    // 目标角度
    sChart targetAngle;
    // 当前速度
    sChart currentVelocity;
    // 目标速度
    sChart targetVelocity;
    // 当前电压
    sChart targetVoltage;
    // 更新状态
    bool chatsUpdStatus = false;
    double x_old = 0;
    double x = 0;
    // 等待
    std::condition_variable sleepCv;
    std::mutex sleepMtx;
};

sCtrlChart *g_KnobCtrlChart = nullptr;

std::thread g_knobChatsThread;

void setPlotMarker(QwtPlot *plot) {
  auto func = [&](int y, const QColor &color) {
      auto marker = new QwtPlotMarker();
      marker->setLineStyle(QwtPlotMarker::HLine);
      marker->setLinePen({color});
      marker->setYValue(y);
      marker->attach(plot);
  };
  // 中值线
  func(0, {211, 216, 237, 180});
  // 中值线上
  func(50, {211, 216, 237, 100});
  // 中值线下
  func(-50, {211, 216, 237, 100});
}

void updatePlot() {
  g_KnobCtrlChart->currentAngle.curve->setSamples(g_KnobCtrlChart->currentAngle.points);
  //g_KnobCtrlChart->currentAngle.curve->attach(g_KnobCtrlChart->Plot);

  g_KnobCtrlChart->targetAngle.curve->setSamples(g_KnobCtrlChart->targetAngle.points);
  // g_KnobCtrlChart->targetAngle.curve->attach(g_KnobCtrlChart->Plot);

  g_KnobCtrlChart->currentVelocity.curve->setSamples(g_KnobCtrlChart->currentVelocity.points);
  //g_KnobCtrlChart->currentVelocity.curve->attach(g_KnobCtrlChart->Plot);

  g_KnobCtrlChart->targetVelocity.curve->setSamples(g_KnobCtrlChart->targetVelocity.points);
  // g_KnobCtrlChart->targetVelocity.curve->attach(g_KnobCtrlChart->Plot);

  //g_KnobCtrlChart->targetVoltage.curve->setSamples(g_KnobCtrlChart->targetVoltage.points.toVector());
  //g_KnobCtrlChart->targetVoltage.curve->attach(g_KnobCtrlChart->Plot);

  g_KnobCtrlChart->Plot->replot();
}

void MainWindow::knobChatsEventStartSampling(bool c) {
  m_ctrlKnobChart->chatsUpdStatus = c;
  g_KnobCtrlChart->sleepCv.notify_all();
}

void MainWindow::knobChatsEventShowTable() {
  if (!g_KnobCtrlChart) {
    return;
  }
  g_KnobCtrlChart->Plot->setAxisScale(QwtPlot::xBottom, 0, 500);
  g_KnobCtrlChart->currentAngle.points.clear();
  g_KnobCtrlChart->targetAngle.points.clear();
  g_KnobCtrlChart->currentVelocity.points.clear();
  g_KnobCtrlChart->targetVelocity.points.clear();
  g_KnobCtrlChart->targetVoltage.points.clear();
  g_KnobCtrlChart->Plot->replot();
  ui->ctrl_checkBox_knob_chart_info_current_angle->setChecked(false);
  ui->ctrl_label_knob_chart_info_current_angle->setText("0°");
  ui->ctrl_label_knob_chart_info_current_velocity->setText("0°");
  ui->ctrl_label_knob_chart_info_current_voltage->setText("0");
  g_KnobCtrlChart->chatsUpdStatus = false;
  g_KnobCtrlChart->x = 0;
  g_KnobCtrlChart->x_old = 0;
}

void MainWindow::knobChatsInit(QWidget *parent) {
  ui->groupBox_charts->setVisible(false);
  connect(ui->ctrl_checkBox_knob_chart_info_current_angle, &QCheckBox::clicked, this,
          &MainWindow::knobChatsEventStartSampling);
  m_ctrlKnobChart = new sCtrlChart;
  g_KnobCtrlChart = m_ctrlKnobChart;
  m_ctrlKnobChart->Plot = new QwtPlot(ui->ctrl_charts_label_bg);
  // 自动刷新
  m_ctrlKnobChart->Plot->setGeometry(0, 0, ui->ctrl_charts_label_bg->width(), ui->ctrl_charts_label_bg->height());
  m_ctrlKnobChart->Plot->autoReplot();
  //m_ctrlPlot->setTitle("电机采样数据");
  m_ctrlKnobChart->Plot->setStyleSheet(
      "color: #d3d8ed;"
      "background-color: #1d2556;"
  );
  // 设置纵坐标信息
  m_ctrlKnobChart->Plot->setAxisScale(QwtPlot::yLeft, -90, 90);
  //m_ctrlPlot->setAxisTitle(QwtPlot::yLeft, "角度量");

  m_ctrlKnobChart->Plot->enableAxis(QwtPlot::yRight, true);
  m_ctrlKnobChart->Plot->setAxisScale(QwtPlot::yRight, -15, 15);
  //m_ctrlPlot->setAxisTitle(QwtPlot::yRight, "速度量");

  m_ctrlKnobChart->Plot->enableAxis(QwtPlot::xBottom, false);
  m_ctrlKnobChart->Plot->setAxisScale(QwtPlot::xBottom, 0, 500);

  setPlotMarker(m_ctrlKnobChart->Plot);

  auto func = [this](sChart &c, QwtAxisId AxisId, const std::string &title, const QColor &color) {
      c.curve = new QwtPlotCurve(title.c_str());
      c.curve->setCurveFitter(new QwtSplineCurveFitter);
      c.curve->setRenderHint(QwtPlotCurve::RenderAntialiased);
      c.curve->setStyle(QwtPlotCurve::Lines);
      //QRandomGenerator *generator = QRandomGenerator::global();
//      double randomValue = generator->generateDouble() * 0.09 + 0.01;
//      QPolygonF points;
//      for (int i = 0; i < 5000; ++i) {
//        points << QPointF(i, qSin(i * randomValue) * 90); // 生成示例数据
//      }
//      c.curve->setSamples(points);
      c.curve->setYAxis(AxisId);
      c.curve->attach(m_ctrlKnobChart->Plot);
      c.curve->setPen(QPen{color, 2});
      auto qlegend = new QwtLegend;
      m_ctrlKnobChart->Plot->insertLegend(qlegend, QwtPlot::BottomLegend);
  };
//  func(m_ctrlKnobChart->currentAngle, QwtPlot::yLeft, "当前角度(左)", QColor{255, 199, 69});
//  func(m_ctrlKnobChart->targetAngle, QwtPlot::yLeft, "目标角度(左)", QColor{255, 144, 69});
//  func(m_ctrlKnobChart->currentVelocity, QwtPlot::yRight, "当前速度(右)", QColor{98, 98, 128});
//  func(m_ctrlKnobChart->targetVelocity, QwtPlot::yRight, "目标速度(右)", QColor{148, 148, 212});
  func(m_ctrlKnobChart->currentAngle, QwtPlot::yLeft, "当前角度(左)", QColor{0, 173, 239});
  func(m_ctrlKnobChart->targetAngle, QwtPlot::yLeft, "目标角度(左)", QColor{141, 198, 63});
  func(m_ctrlKnobChart->currentVelocity, QwtPlot::yRight, "当前速度(右)", QColor{155, 89, 182});
  func(m_ctrlKnobChart->targetVelocity, QwtPlot::yRight, "目标速度(右)", QColor{255, 127, 39});

  g_knobChatsThread = std::thread{
      [this]() {
          std::unique_lock<std::mutex> lock(g_KnobCtrlChart->sleepMtx);
          while (true) {
            if (!g_KnobCtrlChart->chatsUpdStatus) {
              g_KnobCtrlChart->sleepCv.wait(lock, [] { return g_KnobCtrlChart->chatsUpdStatus; });
            }
            try {
              if (!g_mainWind->checkCtrlConnect()) {
                g_KnobCtrlChart->chatsUpdStatus = false;
                // 未连接设备
                continue;
              }
              //std::cout << "upd status" << std::endl;
              g_KnobCtrlChart->x++;
              Lib::HWDeviceTools tools;
              auto status = tools.GetKnobStatus(g_mainWind->getCtrlConnectDev());
              sChart currentAngle;
              // 目标角度
              g_KnobCtrlChart->currentAngle.points.push_back(
                  {g_KnobCtrlChart->x, Lib::NormalizeAngle(status.current_angle)});
              // 目标角度
              g_KnobCtrlChart->targetAngle.points.push_back(
                  {g_KnobCtrlChart->x, Lib::NormalizeAngle(status.target_angle)});
              // 当前速度
              g_KnobCtrlChart->currentVelocity.points.push_back({g_KnobCtrlChart->x, status.current_velocity});
              // 目标速度
              g_KnobCtrlChart->targetVelocity.points.push_back({g_KnobCtrlChart->x, status.target_velocity});
              // 当前电压
              g_KnobCtrlChart->targetVoltage.points.push_back({g_KnobCtrlChart->x, status.target_voltage});
              std::cout
                  << fmt::format(
                      "convertToCoordinateSystem:{} currentAngle:{} targetAngle:{} currentVelocity:{} targetVelocity:{} targetVoltage:{}",
                      Lib::NormalizeAngle(status.current_angle),
                      Lib::NormalizeAngle(status.current_angle),
                      status.target_angle,
                      status.current_velocity,
                      status.target_velocity,
                      status.target_voltage
                  ) << std::endl;
              if (g_KnobCtrlChart->targetAngle.points.size() > 500) {
                g_KnobCtrlChart->x_old++;
              }
              g_KnobCtrlChart->Plot->setAxisScale(QwtPlot::xBottom, g_KnobCtrlChart->x_old, g_KnobCtrlChart->x);
              double fAgent=float (int(status.current_angle*100)%36000)/100;
              ui->ctrl_label_knob_chart_info_current_angle->setText(fmt::format("{:.2f}°({:.2f}°)",status.current_angle,fAgent).c_str());
              ui->ctrl_label_knob_chart_info_current_velocity->setText(
                  QString::number(status.current_velocity, 'f', 2) + "°");
              ui->ctrl_label_knob_chart_info_current_voltage->setText(
                  QString::number(status.target_voltage, 'f', 2));

              updatePlot();
            } catch (Lib::DeviceException &e) {
              // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
            } catch (std::exception &e) {
              // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
            }
          }

      }
  };
}