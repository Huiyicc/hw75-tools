#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

size_t ConvertToUf2(const std::string &BinFile, const std::string &Uf2File) {
  int familyid = 1467308631;
  auto appstartaddr = 0x8010000; // stm32f405
  auto UF2_MAGIC_START0 = 0x0A324655; // "UF2\n"
  auto UF2_MAGIC_START1 = 0x9E5D5157; // Randomly selected
  auto UF2_MAGIC_END = 0x0AB16F30; // Ditto
  std::ifstream binFile(BinFile, std::ios::binary);
  auto file_content = std::string((std::istreambuf_iterator<char>(binFile)), std::istreambuf_iterator<char>());
  std::vector<uint8_t> datapadding;
  while (datapadding.size() < 512 - 256 - 32 - 4) {
    datapadding.push_back(0);
    datapadding.push_back(0);
    datapadding.push_back(0);
    datapadding.push_back(0);
  }
  auto num_blocks = static_cast<size_t>((file_content.size() + 255) / 256);
  std::vector<uint8_t> outp;
  for (size_t blockno = 0; blockno < num_blocks; blockno++) {
    auto ptr = 256 * blockno;
    std::vector<uint8_t> chunk(file_content.begin() +ptr, file_content.begin() + ptr + 256);
    auto flags = 0x0;
    flags |= 0x2000;
    std::vector<uint8_t> hd(32, 0);
    *(reinterpret_cast<uint32_t *>(&hd[0])) = UF2_MAGIC_START0;
    *(reinterpret_cast<uint32_t *>(&hd[0])) = UF2_MAGIC_START0;
    *(reinterpret_cast<uint32_t *>(&hd[4])) = UF2_MAGIC_START1;
    *(reinterpret_cast<uint32_t *>(&hd[8])) = flags;
    *(reinterpret_cast<uint32_t *>(&hd[12])) = ptr + appstartaddr;
    *(reinterpret_cast<uint32_t *>(&hd[16])) = 256;
    *(reinterpret_cast<uint32_t *>(&hd[20])) = blockno;
    *(reinterpret_cast<uint32_t *>(&hd[24])) = num_blocks;
    *(reinterpret_cast<uint32_t *>(&hd[28])) = familyid;
    while (chunk.size() < 256) {
      chunk.push_back(0);
    }
    std::vector<uint8_t> block;
    block.insert(block.end(), hd.begin(), hd.end());
    block.insert(block.end(), chunk.begin(), chunk.end());
    block.insert(block.end(), datapadding.begin(), datapadding.end());
    std::vector<uint8_t> tmp(4,0);
    *(reinterpret_cast<uint32_t *>(&tmp[0])) = UF2_MAGIC_END;
    block.insert(block.end(), tmp.begin(), tmp.end());
    if (block.size() != 512) {
      throw std::runtime_error("block size error");
    }
    outp.insert(outp.end(), block.begin(), block.end());
  }
  std::ofstream uf2File(Uf2File, std::ios::binary);
  uf2File.write(reinterpret_cast<const char *>(outp.data()), outp.size());
  uf2File.close();
  binFile.close();
  return outp.size();
}

void check(const std::string &file1, const std::string &file2) {
  std::ifstream f1(file1, std::ios::binary);
  std::ifstream f2(file2, std::ios::binary);
  std::vector<uint8_t> v1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
  std::vector<uint8_t> v2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());
  size_t c = 0;
  c = std::min(v1.size(), v2.size());
//  if (v1.size() != v2.size()) {
//    std::cout << "size not equal" << std::endl;
//    return;
//  }
  for (size_t i = 0; i < c; i++) {
    if (v1[i] != v2[i]) {
      std::cout << "not equal at " << i << " " << " C:" << int(v1[i]) << " != " << "py:" << int(v2[i]) << std::endl;
      return;
    }
  }
  std::cout << "equal" << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    fprintf(stderr, "USAGE: %s file.bin [file.uf2]\n", argv[0]);
    return 1;
  }
  // argv[2]: file.uf2
  // argv[3]: file.bin
  if (atoi(argv[1])==1) {
    check(argv[2], argv[3]);
  } else {
    std::cout << "out size: " << ConvertToUf2(argv[3], argv[2]) << std::endl;
  }


  return 0;
}
