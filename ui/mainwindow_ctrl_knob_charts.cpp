//
// Created by 19254 on 2023/11/17.
//
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ui/widget/MsgBox.hpp"
#include "knobaligndialog.h"

#include <QFileDialog>
#include <QThread>
#include <QRandomGenerator>

#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_plot_marker.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_curve_fitter.h>
#include <Qwt/qwt_spline_curve_fitter.h>

struct sChart {
    QwtPlotCurve *curve = nullptr;
};

struct sCtrlChart {
    QwtPlot *Plot = nullptr;
    // 当前角度
    sChart current_angle;
    // 目标角度
    sChart target_angle;
    // 当前速度
    sChart current_velocity;
    // 目标速度
    sChart target_velocity;
    // 当前电压
    float target_voltage = 0;
};

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

}

void MainWindow::knobChatsEventStartSampling(bool c) {
  if (c) {

    try {
      if (m_modelConnectStatus.find(HW_MODEL_NAME_CTRL) == m_modelConnectStatus.end()) {
        // 未连接设备
        return;
      }
      Lib::HWDeviceTools tools;
      tools.GetKnobStatus(m_modelConnectStatus[HW_MODEL_NAME_CTRL]);
      return;
    } catch (Lib::DeviceException &e) {
      // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    } catch (std::exception &e) {
      // QMessageBox::critical(this, "错误", QString::fromStdString(e.what()));
    }

  }
}

void MainWindow::knobChatsInit(QWidget *parent) {
  ui->groupBox_charts->setVisible(false);
  connect(ui->ctrl_checkBox_knob_chart_info_current_angle,&QCheckBox::clicked,this,&MainWindow::knobChatsEventStartSampling);
  m_ctrlKnobChart = new sCtrlChart;
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
  m_ctrlKnobChart->Plot->setAxisScale(QwtPlot::yRight, -10, 10);
  //m_ctrlPlot->setAxisTitle(QwtPlot::yRight, "速度量");

  m_ctrlKnobChart->Plot->enableAxis(QwtPlot::xBottom, false);
  m_ctrlKnobChart->Plot->setAxisScale(QwtPlot::xBottom, 0, 500);

  setPlotMarker(m_ctrlKnobChart->Plot);

  auto func = [this](sChart &c, const std::string &title, const QColor &color) {
      c.curve = new QwtPlotCurve(title.c_str());
      c.curve->setCurveFitter(new QwtSplineCurveFitter);
      c.curve->setRenderHint(QwtPlotCurve::RenderAntialiased);
      c.curve->setStyle(QwtPlotCurve::Lines);
      QRandomGenerator *generator = QRandomGenerator::global();
      double randomValue = generator->generateDouble() * 0.09 + 0.01;
      QPolygonF points;
      for (int i = 0; i < 5000; ++i) {
        points << QPointF(i, qSin(i * randomValue) * 90); // 生成示例数据
      }
      c.curve->setSamples(points);
      c.curve->attach(m_ctrlKnobChart->Plot);
      c.curve->setPen(QPen{color, 2});
      auto qlegend = new QwtLegend;
      m_ctrlKnobChart->Plot->insertLegend(qlegend, QwtPlot::BottomLegend);
  };
  func(m_ctrlKnobChart->current_angle, "当前角度(左)", QColor{255, 199, 69});
  func(m_ctrlKnobChart->target_angle, "目标角度(左)", QColor{255, 144, 69});
  func(m_ctrlKnobChart->current_velocity, "当前速度(右)", QColor{98, 98, 128});
  func(m_ctrlKnobChart->target_velocity, "目标速度(右)", QColor{148, 148, 212});

}