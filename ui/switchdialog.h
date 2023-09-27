#ifndef SWITCHDIALOG_H
#define SWITCHDIALOG_H

#include <QDialog>

namespace Ui {
class SwitchDialog;
}

class SwitchDialog : public QDialog {
Q_OBJECT

public:
    explicit SwitchDialog(std::map<std::string, int> &data, std::shared_ptr<int> &result,QWidget *parent = nullptr);

    ~SwitchDialog();


    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;


private:

    void checkSuccess(bool checked);

    bool eventFilter(QObject *obj, QEvent *event);

    std::map<std::string, int> data;
    std::shared_ptr<int> call = nullptr;

    Ui::SwitchDialog *ui;
    QPoint z;
};

#endif // SWITCHDIALOG_H
