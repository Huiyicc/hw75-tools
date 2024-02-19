//
// Created by 19254 on 2023/11/1.
//
#include <QMessageBox>
#include "MsgBox.hpp"

namespace MsgBox {

int information(QWidget *parent, const QString &title,
                const QString &text) {
	QMessageBox msgBox(parent);
	msgBox.setWindowTitle(title);
	msgBox.setText(text);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setStyleSheet(""
						 "QLabel{color: white;}"
						 "QPushButton{\n"
	                     "background:qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:0 #3fd38f, stop:1 #50b4cb);\n"
	                     "color: rgb(211, 216, 237);\n"
	                     "border-radius: 3px;\n"
	                     "border: none;\n"
	                     "}\n"
	                     "QPushButton:focus {\n"
	                     "    outline: none;\n"
	                     "}");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	return msgBox.exec();
};

int critical(QWidget *parent, const QString &title,
             const QString &text) {
	QMessageBox msgBox(parent);
	msgBox.setWindowTitle(title);
	msgBox.setText(text);
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setStyleSheet(""
	                     "QLabel{color: white;}"
	                     "QPushButton{\n"
	                     "background:qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:0 #3fd38f, stop:1 #50b4cb);\n"
	                     "color: rgb(211, 216, 237);\n"
	                     "border-radius: 3px;\n"
	                     "border: none;\n"
	                     "}\n"
	                     "QPushButton:focus {\n"
	                     "    outline: none;\n"
	                     "}");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);
	return msgBox.exec();
}

int question(QWidget *parent, const QString &title,
             const QString &text) {
  QMessageBox msgBox(parent);
  msgBox.setWindowTitle(title);
  msgBox.setText(text);
  msgBox.setIcon(QMessageBox::Question);
  msgBox.setStyleSheet(""
                       "QLabel{color: white;}"
                       "QPushButton{\n"
                       "background:qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:0 #3fd38f, stop:1 #50b4cb);\n"
                       "color: rgb(211, 216, 237);\n"
                       "border-radius: 3px;\n"
                       "border: none;\n"
                       "}\n"
                       "QPushButton:focus {\n"
                       "    outline: none;\n"
                       "}");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  return msgBox.exec();
}

}