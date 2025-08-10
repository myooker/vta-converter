#ifndef RLE_H
#define RLE_H
#include <cstdint>
#include <string>
#include <vector>
#include <opencv2/core/hal/interface.h>

struct RLE_JEW;

void rleEncode(const std::vector<int> &data, std::string &encode);
void rleEncode(const std::vector<int> &data, std::vector<std::string> &encode);
void rleEncode(const std::vector<int> &data, std::vector<std::pair<uint16_t, uint16_t>> &encode);
void rleEncode2(const std::vector<int> &data, std::vector<std::pair<uint16_t, uint16_t>> &encode);
void rleEncode(const std::vector<int> &data, std::vector<RLE_JEW> &encode);

struct RLE_JEW {
    uint16_t count{};
    uint16_t grayVal{};
    uint16_t repeat{};
};

#endif //RLE_H
