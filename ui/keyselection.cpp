#include "keyselection.h"
#include "ui_keyselection.h"
#include <QGraphicsDropShadowEffect>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include "utils/Log.hpp"

#define DEFAULT_BUTTON_WIDTH 42
#define DEFAULT_BUTTON_HEIGHT 42

#define NORMAL_INTERVAL_WIDTH 3
#define NORMAL_INTERVAL_HEIGHT 5

#define DSPLIT_BUTTON_WIDTH 20

#define NORMAL_BUTTON(TEXT, ADDITIONAL, KEY) \
  {                                          \
    .type = KeyLayoutType::NormalButtons,    \
    .width = DEFAULT_BUTTON_WIDTH,           \
    .height = DEFAULT_BUTTON_HEIGHT,         \
    .text = TEXT,                            \
    .additional = ADDITIONAL,                \
    .key = KEY,                              \
  }

#define NORMAL_BUTTON_W(TEXT, ADDITIONAL, KEY, W) \
  {                                               \
    .type = KeyLayoutType::NormalButtons,         \
    .width = W,                                   \
    .height = DEFAULT_BUTTON_HEIGHT,              \
    .text = TEXT,                                 \
    .additional = ADDITIONAL,                     \
    .key = KEY,                                   \
  }

#define NORMAL_BUTTON_H(TEXT, ADDITIONAL, KEY, H) \
  {                                               \
    .type = KeyLayoutType::NormalButtons,         \
    .width = DEFAULT_BUTTON_WIDTH,                \
    .height = H,                                  \
    .text = TEXT,                                 \
    .additional = ADDITIONAL,                     \
    .key = KEY,                                   \
  }

#define SPLIT_BUTTON()               \
  {                                  \
    .type = KeyLayoutType::Split,    \
    .width = DSPLIT_BUTTON_WIDTH,    \
    .height = DEFAULT_BUTTON_HEIGHT, \
    .text = "",                      \
    .additional = "",                \
    .key = Default                   \
  }

#define SPLIT_BUTTON_W(W)            \
  {                                  \
    .type = KeyLayoutType::Split,    \
    .width = W,                      \
    .height = DEFAULT_BUTTON_HEIGHT, \
    .text = "",                      \
    .additional = "",                \
    .key = Default                   \
  }

static std::vector<std::vector<KeyLayout>> FullKeyboardLayout = {
    // 6行
    {
        NORMAL_BUTTON("Esc", "", KeyBoardValue::ESC),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("F1", "", KeyBoardValue::F1),
        NORMAL_BUTTON("F2", "", KeyBoardValue::F2),
        NORMAL_BUTTON("F3", "", KeyBoardValue::F3),
        NORMAL_BUTTON("F4", "", KeyBoardValue::F4),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("F5", "", KeyBoardValue::F5),
        NORMAL_BUTTON("F6", "", KeyBoardValue::F6),
        NORMAL_BUTTON("F7", "", KeyBoardValue::F7),
        NORMAL_BUTTON("F8", "", KeyBoardValue::F8),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("F9", "", KeyBoardValue::F9),
        NORMAL_BUTTON("F10", "", KeyBoardValue::F10),
        NORMAL_BUTTON("F11", "", KeyBoardValue::F11),
        NORMAL_BUTTON("F12", "", KeyBoardValue::F12),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("Print", "Screen", KeyBoardValue::PRINT_SCREEN),
        NORMAL_BUTTON("Scroll", "Lock", KeyBoardValue::SCROLL_LOCK),
        NORMAL_BUTTON("Pause", "", KeyBoardValue::PAUSE),
    },
    {
        NORMAL_BUTTON("~", "`", KeyBoardValue::TILDE),
        NORMAL_BUTTON("!", "1", KeyBoardValue::ONE),
        NORMAL_BUTTON("@", "2", KeyBoardValue::TWO),
        NORMAL_BUTTON("#", "3", KeyBoardValue::THREE),
        NORMAL_BUTTON("$", "4", KeyBoardValue::FOUR),
        NORMAL_BUTTON("%", "5", KeyBoardValue::FIVE),
        NORMAL_BUTTON("^", "6", KeyBoardValue::SIX),
        NORMAL_BUTTON("&", "7", KeyBoardValue::SEVEN),
        NORMAL_BUTTON("*", "8", KeyBoardValue::EIGHT),
        NORMAL_BUTTON("(", "9", KeyBoardValue::NINE),
        NORMAL_BUTTON(")", "0", KeyBoardValue::ZERO),
        NORMAL_BUTTON("_", "-", KeyBoardValue::MINUS),
        NORMAL_BUTTON("+", "=", KeyBoardValue::EQUAL),
        NORMAL_BUTTON_W("Back", "Space", KeyBoardValue::BACKSPACE, DEFAULT_BUTTON_WIDTH + 26),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("Insert", "", KeyBoardValue::INSERT),
        NORMAL_BUTTON("Home", "", KeyBoardValue::HOME),
        NORMAL_BUTTON("Page", "Up", KeyBoardValue::PAGE_UP),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("Num", "Lock", KeyBoardValue::NUM_LOCK),
        NORMAL_BUTTON("/", "", KeyBoardValue::NUM_SLASH),
        NORMAL_BUTTON("*", "", KeyBoardValue::NUM_ASTERISK),
        NORMAL_BUTTON("-", "", KeyBoardValue::NUM_MINUS),
    },
    {
        NORMAL_BUTTON_W("Tab", "", KeyBoardValue::TAB, DEFAULT_BUTTON_WIDTH + 14),
        NORMAL_BUTTON("Q", "", KeyBoardValue::Q),
        NORMAL_BUTTON("W", "", KeyBoardValue::W),
        NORMAL_BUTTON("E", "", KeyBoardValue::E),
        NORMAL_BUTTON("R", "", KeyBoardValue::R),
        NORMAL_BUTTON("T", "", KeyBoardValue::T),
        NORMAL_BUTTON("Y", "", KeyBoardValue::Y),
        NORMAL_BUTTON("U", "", KeyBoardValue::U),
        NORMAL_BUTTON("I", "", KeyBoardValue::I),
        NORMAL_BUTTON("O", "", KeyBoardValue::O),
        NORMAL_BUTTON("P", "", KeyBoardValue::P),
        NORMAL_BUTTON("{", "[", KeyBoardValue::LEFT_BRACKET),
        NORMAL_BUTTON("}", "]", KeyBoardValue::RIGHT_BRACKET),
        NORMAL_BUTTON_W("|", "\\", KeyBoardValue::BACKSLASH, DEFAULT_BUTTON_WIDTH + 12),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("Delete", "", KeyBoardValue::DELETE),
        NORMAL_BUTTON("End", "", KeyBoardValue::END),
        NORMAL_BUTTON("Page", "Down", KeyBoardValue::PAGE_DOWN),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("7", "", KeyBoardValue::NUM_SEVEN),
        NORMAL_BUTTON("8", "", KeyBoardValue::NUM_EIGHT),
        NORMAL_BUTTON("9", "", KeyBoardValue::NUM_NINE),
        NORMAL_BUTTON_H("+", "", KeyBoardValue::NUM_PLUS, DEFAULT_BUTTON_HEIGHT * 2 + NORMAL_INTERVAL_HEIGHT),
    },
    {
        NORMAL_BUTTON_W("Caps", "Lock", KeyBoardValue::CAPS_LOCK, int(DEFAULT_BUTTON_WIDTH * 1.9)),
        NORMAL_BUTTON("A", "", KeyBoardValue::A),
        NORMAL_BUTTON("S", "", KeyBoardValue::S),
        NORMAL_BUTTON("D", "", KeyBoardValue::D),
        NORMAL_BUTTON("F", "", KeyBoardValue::F),
        NORMAL_BUTTON("G", "", KeyBoardValue::G),
        NORMAL_BUTTON("H", "", KeyBoardValue::H),
        NORMAL_BUTTON("J", "", KeyBoardValue::J),
        NORMAL_BUTTON("K", "", KeyBoardValue::K),
        NORMAL_BUTTON("L", "", KeyBoardValue::L),
        NORMAL_BUTTON(":", ";", KeyBoardValue::SEMICOLON),
        NORMAL_BUTTON("\"", "'", KeyBoardValue::QUOTE),
        NORMAL_BUTTON_W("Enter", "", KeyBoardValue::ENTER, int(DEFAULT_BUTTON_WIDTH * 1.8) + 1),
        SPLIT_BUTTON(),
        SPLIT_BUTTON_W(DEFAULT_BUTTON_WIDTH),
        SPLIT_BUTTON_W(DEFAULT_BUTTON_WIDTH),
        SPLIT_BUTTON_W(DEFAULT_BUTTON_WIDTH),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("4", "", KeyBoardValue::NUM_FOUR),
        NORMAL_BUTTON("5", "", KeyBoardValue::NUM_FIVE),
        NORMAL_BUTTON("6", "", KeyBoardValue::NUM_SIX),
    },
    {
        NORMAL_BUTTON_W("Shift", "", KeyBoardValue::LEFT_SHIFT, int(DEFAULT_BUTTON_WIDTH * 2.4)),
        NORMAL_BUTTON("Z", "", KeyBoardValue::Z),
        NORMAL_BUTTON("X", "", KeyBoardValue::X),
        NORMAL_BUTTON("C", "", KeyBoardValue::C),
        NORMAL_BUTTON("V", "", KeyBoardValue::V),
        NORMAL_BUTTON("B", "", KeyBoardValue::B),
        NORMAL_BUTTON("N", "", KeyBoardValue::N),
        NORMAL_BUTTON("M", "", KeyBoardValue::M),
        NORMAL_BUTTON("<", ",", KeyBoardValue::COMMA),
        NORMAL_BUTTON(">", ".", KeyBoardValue::PERIOD),
        NORMAL_BUTTON("?", "/", KeyBoardValue::SLASH),
        NORMAL_BUTTON_W("Shift", "", KeyBoardValue::RIGHT_SHIFT, int(DEFAULT_BUTTON_WIDTH * 2.4)),
        SPLIT_BUTTON(),
        SPLIT_BUTTON_W(DEFAULT_BUTTON_WIDTH),
        NORMAL_BUTTON("Up", "", KeyBoardValue::UP),
        SPLIT_BUTTON_W(DEFAULT_BUTTON_WIDTH),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("1", "", KeyBoardValue::NUM_ONE),
        NORMAL_BUTTON("2", "", KeyBoardValue::NUM_TWO),
        NORMAL_BUTTON("3", "", KeyBoardValue::NUM_THREE),
        NORMAL_BUTTON_H("Enter", "", KeyBoardValue::NUM_ENTER, DEFAULT_BUTTON_HEIGHT * 2 + NORMAL_INTERVAL_HEIGHT),
    },
    {
        NORMAL_BUTTON_W("Ctrl", "", KeyBoardValue::LEFT_CTRL, int(DEFAULT_BUTTON_WIDTH * 1.5)),
        NORMAL_BUTTON("Win", "", KeyBoardValue::LEFT_SUPER),
        NORMAL_BUTTON("Alt", "", KeyBoardValue::LEFT_ALT),
        NORMAL_BUTTON_W("Space", "", KeyBoardValue::SPACE, DEFAULT_BUTTON_WIDTH * 7 + NORMAL_INTERVAL_WIDTH),
        NORMAL_BUTTON("Alt", "", KeyBoardValue::RIGHT_ALT),
        NORMAL_BUTTON("Win", "", KeyBoardValue::RIGHT_SUPER),
        NORMAL_BUTTON_W("Menu", "", KeyBoardValue::MENU, int(DEFAULT_BUTTON_WIDTH * 1.5)),
        NORMAL_BUTTON("Ctrl", "", KeyBoardValue::RIGHT_CTRL),
        SPLIT_BUTTON(),
        NORMAL_BUTTON("Left", "", KeyBoardValue::LEFT),
        NORMAL_BUTTON("Down", "", KeyBoardValue::DOWN),
        NORMAL_BUTTON("Right", "", KeyBoardValue::RIGHT),
        SPLIT_BUTTON(),
        NORMAL_BUTTON_W("0", "", KeyBoardValue::NUM_ZERO, DEFAULT_BUTTON_WIDTH * 2 + NORMAL_INTERVAL_WIDTH),
        NORMAL_BUTTON(".", "", KeyBoardValue::NUM_PERIOD),
    },
};

KeySelection::KeySelection(QWidget *parent) : QDialog(parent),
                                              ui(new Ui::KeySelection) {
  ui->setupUi(this);
  //设置无边框,去默认的标题栏
  setWindowFlag(Qt::FramelessWindowHint);    //无边框
  setAttribute(Qt::WA_TranslucentBackground);//窗口透明

  //设置投影效果
  auto windowShadow = new QGraphicsDropShadowEffect(this);//阴影效果
  windowShadow->setBlurRadius(11);
  windowShadow->setColor(QColor(0, 0, 0, 120));
  windowShadow->setOffset(0, 0);
  ui->frame->setGraphicsEffect(windowShadow);
  ui->testbutton->setVisible(false);
  m_keyLayouts = FullKeyboardLayout;
  updateKeyLayouts();
}

KeySelection::~KeySelection() {
  delete ui;
}

void KeySelection::updateKeyLayouts() {
  auto fp = ui->tabWidget->widget(0);
  int y = 20;
  for (auto &row: m_keyLayouts) {
    int x = 0;
    for (auto &layout: row) {
      auto button = new QPushButton(layout.text + "\n" + layout.additional, fp);
      if (layout.type != KeyLayoutType::Split) {
        button->setStyleSheet(R"(
                                  QPushButton {
                                      border: 1px solid #a4b0be;
                                      border-radius: 5px;
                                      background-color: #fff;
                                      color: #747d8c;
                                  }

                                  QPushButton:hover {
                                      background-color: #ecf0f1;
                                  }

                                  QPushButton:pressed {
                                      background-color: #bdc3c7;
                                  }
)");
        button->setProperty("keycode", QVariant(layout.key));
        button->setCursor(Qt::PointingHandCursor);
        connect(button, &QPushButton::clicked, this, &KeySelection::onKeyBoardClicked);
      }
      button->setGeometry(x, y, layout.width, layout.height);
      x += layout.width + NORMAL_INTERVAL_WIDTH;
    }
    y += DEFAULT_BUTTON_HEIGHT + NORMAL_INTERVAL_HEIGHT;
  }
};

void KeySelection::mouseMoveEvent(QMouseEvent *event) {
  QWidget::mouseMoveEvent(event);
  QPoint y = event->globalPosition().toPoint();
  // QPoint y = event->globalPos();
  QPoint x = y - z;
  this->move(x);
}

void KeySelection::mousePressEvent(QMouseEvent *event) {
  QWidget::mousePressEvent(event);
  QPoint x = this->geometry().topLeft();
  QPoint y = event->globalPosition().toPoint();
  //QPoint y = event->globalPos();
  z = y - x;
}

void KeySelection::mouseReleaseEvent(QMouseEvent *event) {
  QWidget::mouseReleaseEvent(event);
}

void KeySelection::onKeyBoardClicked(bool) {
  auto btn = qobject_cast<QPushButton *>(sender());
  auto key = btn->property("keycode").toInt();
  PrintDebug("key: {}", key);
}
