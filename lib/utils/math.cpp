//
// Created by HuiYi on 2023/5/28.
//
#include "math.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <random>

namespace Lib {
int RandomNumber(int min, int max) {
  // 创建一个随机设备以生成非确定性种子
  std::random_device rd;
  // 使用随机设备获取一个种子值，用于随机数引擎
  std::mt19937 rng(rd());
  // 创建一个指定范围的均匀分布
  std::uniform_int_distribution<int> distribution(min, max);
  // 在指定范围内生成一个随机数
  int result = distribution(rng);
  return result;
};

double NormalizeAngle(double angle) {
  auto tmpM = int(angle * 100);
  double decimal = double(tmpM % 100) / 100;
  auto absAngle = abs(tmpM / 100);
  absAngle = absAngle % 360;
  if (absAngle == 180 || absAngle == 0) { return 0; }
  double rest;
  if (absAngle < 180) {
    if (absAngle < 90) {
      rest = absAngle;
    } else {
      rest = 90 - (absAngle - 90);
    }
  } else {
    absAngle -= 180;
    if (absAngle < 90) {
      rest = -absAngle;
    } else {
      rest = -(90 - (absAngle - 90));
    }
  }
  return rest + decimal;
}
}
