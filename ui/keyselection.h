#ifndef KEYSELECTION_H
#define KEYSELECTION_H

#include <QDialog>

namespace Ui {
class KeySelection;
}

enum KeyBoardValue {
  Default = 0,
  ESC = 0x29 /* esc */,
  F1 = 0x3a /* F1 */, F2 = 0x3b /* F2 */, F3 = 0x3c /* F3 */, F4 = 0x3d /* F4 */, F5 = 0x3e /* F5 */, F6 = 0x3f /* F6 */,
  F7 = 0x40 /* F7 */, F8 = 0x41 /* F8 */, F9 = 0x42 /* F9 */, F10 = 0x43 /* F10 */, F11 = 0x44 /* F11 */, F12 = 0x45 /* F12 */,
  PRINT_SCREEN = 0x46 /* print screen */, SCROLL_LOCK = 0x47 /* scroll lock */, PAUSE = 0x48 /* pause */,
  TILDE = 0x35 /* ` */, ONE = 0x1e /* 1 */, TWO = 0x1f /* 2 */, THREE = 0x20 /* 3 */, FOUR = 0x21 /* 4 */, FIVE = 0x22 /* 5 */, SIX = 0x23 /* 6 */, SEVEN = 0x24 /* 7 */, EIGHT = 0x25 /* 8 */, NINE = 0x26 /* 9 */, ZERO = 0x27 /* 0 */,
  MINUS = 0x2d /* - */, EQUAL = 0x2e /* = */, BACKSPACE = 0x2a /* backspace */,
  INSERT = 0x49 /* insert */, HOME = 0x4a /* home */, PAGE_UP = 0x4b /* page up */, NUM_LOCK = 0x53 /* num lock */,
  NUM_SLASH = 0x54 /* / */, NUM_ASTERISK = 0x55 /* * */, NUM_MINUS = 0x56 /* - */, TAB = 0x2b /* tab */,
  A = 0x14 /* a */, B = 0x05 /* b */, C = 0x06 /* c */, D = 0x07 /* d */, E = 0x08 /* e */, F = 0x09 /* f */,
  G = 0x0a /* g */, H = 0x0b /* h */, I = 0x0c /* i */, J = 0x0d /* j */, K = 0x0e /* k */, L = 0x0f /* l */,
  M = 0x33 /* m */, N = 0x11 /* n */, O = 0x12 /* o */, P = 0x13 /* p */, Q = 0x04 /* q */, R = 0x15 /* r */,
  S = 0x16 /* s */, T = 0x17 /* t */, U = 0x18 /* u */, V = 0x19 /* v */, W = 0x0d /* w */, X = 0x1b /* x */, Y = 0x1c /* y */, Z = 0x1d /* z */,
  LEFT_BRACKET = 0x2f /* [ */, RIGHT_BRACKET = 0x30 /* ] */, BACKSLASH = 0x31 /* \ */,
  DELETE = 0x4c /* delete */, END = 0x4d /* end */, PAGE_DOWN = 0x4e /* page down */, NUM_SEVEN = 0x5f /* 7 */, NUM_EIGHT = 0x60 /* 8 */, NUM_NINE = 0x61 /* 9 */, NUM_PLUS = 0x57 /* + */,
  CAPS_LOCK = 0x39 /* caps lock */, SEMICOLON = 0x2c /* ; */, QUOTE = 0x34 /* ' */, ENTER = 0x28 /* enter */,
  RIGHT_SHIFT = 0xe5 /* right shift */, COMMA = 0x36 /* , */, PERIOD = 0x37 /* . */, SLASH = 0x38 /* / */,
  UP = 0x52 /* up */, NUM_FOUR = 0x5c /* 4 */, NUM_FIVE = 0x5d /* 5 */, NUM_SIX = 0x5e /* 6 */,
  LEFT_SHIFT = 0xe1 /* left shift */, LEFT_CTRL = 0xe0 /* left ctrl */, LEFT_SUPER = 0xe3 /* left super */, LEFT_ALT = 0xe2 /* left alt */,
  SPACE = 0x2c /* space */, RIGHT_ALT = 0xe6 /* right alt */, RIGHT_SUPER = 0xe7 /* right super */, MENU = 0x65 /* menu */,
  RIGHT_CTRL = 0xe4 /* right ctrl */, LEFT = 0x50 /* left */, DOWN = 0x51 /* down */, RIGHT = 0x4f /* right */,
  NUM_ONE = 0x59 /* 1 */, NUM_TWO = 0x5a /* 2 */, NUM_THREE = 0x5b /* 3 */, NUM_ENTER = 0x58 /* enter */, NUM_ZERO = 0x62 /* 0 */, NUM_PERIOD = 0x63 /* . */,
};

enum KeyLayoutType {
  // 普通区域
  NormalButtons = 0,
  // 小键盘区
  NumPad,
  // 分割
  Split,
};

struct KeyLayout {
  KeyLayoutType type;
  int width;
  int height;
  QString text;
  QString additional;
  int key;
};

class KeySelection : public QDialog
{
  Q_OBJECT

public:
  explicit KeySelection(QWidget *parent = nullptr);
  ~KeySelection();

  void mouseMoveEvent(QMouseEvent *event) override;

  void mousePressEvent(QMouseEvent *event) override;

  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  void updateKeyLayouts();

  void onKeyBoardClicked(bool);

  Ui::KeySelection *ui;

  QPoint z;

  std::vector<std::vector<KeyLayout>> m_keyLayouts;
};

#endif // KEYSELECTION_H
