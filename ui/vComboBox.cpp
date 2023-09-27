//
// Created by 19254 on 2023/9/27.
//

#include "vComboBox.h"

void vComboBox::showPopup() {
    emit sigPopup();
    QComboBox::showPopup();
    QWidget *popup = this->findChild<QFrame*>();
    popup->move(popup->x(), popup->y() + 4);
}
void vComboBox::sigPopup(){

}
