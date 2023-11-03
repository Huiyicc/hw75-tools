//
// Created by 19254 on 2023/9/27.
//

#ifndef HW_TOOLS_VCOMBOBOX_H
#define HW_TOOLS_VCOMBOBOX_H


#include <QComboBox>

class vComboBox: public QComboBox {
    void showPopup() override;
    void sigPopup();
};


#endif //HW_TOOLS_VCOMBOBOX_H
