#include "knobaligndialog.h"
#include "ui_knobaligndialog.h"
#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QListView>


KnobAlignDialog::KnobAlignDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KnobAlignDialog)
{
    ui->setupUi(this);

    //设置无边框,去默认的标题栏
    setWindowFlag(Qt::FramelessWindowHint);         //无边框
    setAttribute(Qt::WA_TranslucentBackground);     //窗口透明

    //设置投影效果
    auto windowShadow = new QGraphicsDropShadowEffect(this);        //阴影效果
    windowShadow->setBlurRadius(11);
    windowShadow->setColor(QColor(0, 0, 0, 120));
    windowShadow->setOffset(0, 0);
    ui->frame->setGraphicsEffect(windowShadow);

    knob = new KnobDashboard(this);
    knob->setGeometry(65,90,160,160);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &KnobAlignDialog::close);

}

KnobAlignDialog::~KnobAlignDialog()
{
    if (!knob) {
        delete knob;
    }
    delete ui;
}

