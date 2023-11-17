#ifndef KNOBALIGNDIALOG_H
#define KNOBALIGNDIALOG_H

#include <QDialog>
#include "widget/KnobDashboard.hpp"

namespace Ui {
class KnobAlignDialog;
}

class KnobAlignDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KnobAlignDialog(QWidget *parent = nullptr);
    ~KnobAlignDialog();

private:
    KnobDashboard *knob = nullptr;
    Ui::KnobAlignDialog *ui;
};

#endif // KNOBALIGNDIALOG_H
