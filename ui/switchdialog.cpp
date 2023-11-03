#include <QGraphicsDropShadowEffect>
#include "switchdialog.h"
#include "ui_switchdialog.h"
#include "widget/vComboBox.h"
#include <QMouseEvent>
#include <QLayout>
#include <QListView>
#include <QLineEdit>


SwitchDialog::SwitchDialog(std::map<std::string, int> &data, std::shared_ptr<int> &result, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SwitchDialog) {
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

    ui->comboBox->clear();
    for (auto &item: data) {
        ui->comboBox->addItem(QString::fromStdString(item.first), item.second);
    }
    call = result;

    auto ListView = new QListView;
    ui->comboBox->setView(ListView);
    auto lineEdit = new QLineEdit();
    lineEdit->setReadOnly(true);
    lineEdit->installEventFilter(this);
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->installEventFilter(this);
    ui->comboBox->setLineEdit(lineEdit);

    for (int i = 0; i < ui->comboBox->count(); ++i) {
        ui->comboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
    // 设置下拉列表的最大可见行数，默认为5
    ui->comboBox->setMaxVisibleItems(5);

    ui->comboBox->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui->comboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->pushButton, &QPushButton::clicked, this, &SwitchDialog::close);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &SwitchDialog::checkSuccess);

}

SwitchDialog::~SwitchDialog() {
    delete ui;
}


void SwitchDialog::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    QPoint y = event->globalPosition().toPoint();
    QPoint x = y - z;
    this->move(x);
}

void SwitchDialog::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    QPoint x = this->geometry().topLeft();
    QPoint y = event->globalPosition().toPoint();
    z = y - x;
}

void SwitchDialog::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);
}

void SwitchDialog::checkSuccess(bool checked) {
    *call = ui->comboBox->currentIndex();
    close();
}

bool SwitchDialog::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease && dynamic_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
        ui->comboBox->showPopup();
    }

    return QObject::eventFilter(obj, event);
}
