//
// Created by 19254 on 2023/12/26.
//
#include <QApplication>
#include <thread>
#include <QThread>
#include <iostream>
#include <filesystem>
#include "CLI/CLI11.hpp"
#include "image/Image.hpp"
#include <random>

class MainWindow {
};

MainWindow *g_mainWindow;

struct Args {
    std::string input_file;
    std::string output_file;
    std::string begin;
    std::string end;
    int width = 128;
    int height = 296;
    int threshold = 128;
};

// 生成指定长度的随机字符串
std::string generateRandomString(int length) {
  // 初始化随机数生成器
  std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));

  // 定义字符集
  const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  // 生成随机字符串
  std::string result;
  for (int i = 0; i < length; ++i) {
    result += charset[rng() % charset.length()];
  }

  return result;
}


void outFunc(const Args&args) {
  // 读入图像文件
  // 打开文件
  std::ifstream file(args.input_file, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("error opening file: " + args.input_file);
  }
  // 获取文件大小
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  // 初始化文件转内存
  std::shared_ptr<unsigned char> data(new unsigned char[size], std::default_delete<unsigned char[]>());

  if (!file.read(reinterpret_cast<char *>(data.get()), size)) {
    throw std::runtime_error("Error reading file: " + args.input_file);
  }

  file.close();

  // 创建图像对象
  QByteArray byteArray((const char *) data.get(), size);
  auto imgLib = Lib::Image(QImage::fromData(byteArray));
  auto outData = imgLib.BinaryImgDataToBits(args.threshold, args.width, args.height);
  // 输出流
  std::stringstream ss;
  std::string huid(generateRandomString(10));
  ss << "#ifdef _DEF_" << huid << "_\n";
  ss << "#define _DEF_" << huid << "_\n\n";
  ss << "constexpr unsigned char resources_data []\n\t";
  if (!args.begin.empty()) {
    for (auto & i:args.begin) {
      ss << static_cast<int>(i) << ",";
    }
    ss << "\n\t";
  }
  int n = 1;
  for (auto &c: *outData) {
    ss << static_cast<int>(c) << ",";
    if (n % 32 == 0) {
      ss << "\n\t";
    }
    n++;
  }
  if (!args.end.empty()) {
    for (auto & i:args.end) {
      ss << static_cast<int>(i) << ",";
    }
    ss << "\n";
  }
  ss << "};\n";
  ss << "#endif\n";
  std::filesystem::path path(args.output_file.empty() ? args.input_file : args.output_file);
  // 后缀名
  std::string fileExtension = path.extension().string();
  // 修正后缀
  fileExtension = (fileExtension != ".h" ? ".h" : fileExtension);
  // 输出文件所在的路径
  std::string fileDirectory = args.output_file.empty() ? "./" : path.parent_path().string();
  // 无后缀文件名
  std::string fileNameWithoutExtension = path.stem().string();
  // 输出文件名
  std::string outFileName = fileDirectory + fileNameWithoutExtension + fileExtension;
  // 输出到文件
  std::ofstream outFile(outFileName);

  if (outFile.is_open()) {
    outFile << ss.str();
    // 关闭文件
    outFile.close();
    std::cout << "输出 -> " << outFileName << std::endl;
  } else {
    throw std::runtime_error("无法打开文件 " + outFileName + " 进行写入");
  }

}

int parseArg(int argc, const char **argv) {
  CLI::App app{"图像取模工具"};

  Args a;

  app.add_option("-i,--input", a.input_file, "输入的图片文件")->check(CLI::ExistingFile)->required();
  app.add_option("-o,--output", a.output_file, "输出文件 (暂时只支持h头文件),默认输出到同级目录下.");
  app.add_option("-w,--width", a.width, "输出目标图像的宽度,默认: 128px");
  app.add_option("-e,--height", a.height, "输出目标图像的高度,默认: 296px");
  app.add_option("-t,--threshold", a.threshold, "输出目标图像二值化的阈值,默认: 128");
  app.add_option("-b,--begin", a.begin, "输出文件前缀,文本");
  app.add_option("-d,--end", a.end, "输出文件后缀,文本");

  CLI11_PARSE(app, argc, argv);
  // for (int i=0;i<10;i++) {
  //   a.input_file="C:\\Users\\19254\\Pictures\\Saved Pictures\\much\\"+std::to_string(i)+".bmp";
  //
  // }
  outFunc(a);

  return 0;
}

int main(int argc, char **argv) {
  std::cout<<"hello"<<std::endl;
  try {
    parseArg(argc, (const char **) argv);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
