//
// Created by 19254 on 2023/11/1.
//

#ifndef HW_TOOLS_MSGBOX_HPP
#define HW_TOOLS_MSGBOX_HPP

#include <QWidget>

namespace MsgBox {
int information(QWidget *parent, const QString &title,
                const QString &text);

int critical(QWidget *parent, const QString &title,
             const QString &text);

}

#endif //HW_TOOLS_MSGBOX_HPP
