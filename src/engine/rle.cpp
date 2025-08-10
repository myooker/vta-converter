#include "rle.h"
#include "iostream"

#include <string>
#include <opencv2/core/mat.hpp>

void rleEncode(const std::vector<int> &data, std::string &encode) {
    for (std::size_t i{}; i < data.size(); i++) {
        std::size_t j { i };
        int count{ 1 };
        const auto current { data[i] };

        while (j < data.size()-1 && current == data[j+1]) {
            count++;
            j++;
        }

        encode += "(" + std::to_string(count) + "," + std::to_string(current) + ")";
        i = j;
    }
}

void rleEncode(const std::vector<int> &data, std::vector<std::pair<uint16_t, uint16_t>> &encode) {
    std::pair<uint16_t, uint16_t> encodeData{};
    const auto dataSize{ data.size() };
    encode.reserve(dataSize);

    for (std::size_t i{}; i < dataSize; i++) {
        std::size_t j { i };
        int count{ 1 };
        const auto current { data[i] };

        while (j < dataSize - 1 && current == data[j+1]) {
            count++;
            j++;
        }
        i = j;

        encodeData.first = count;
        encodeData.second = current;

        // if (!encode.empty() && encode.back() == encodeData)
        //     continue;

        encode.push_back(encodeData);
    }
}

void rleEncode2(const std::vector<int> &data, std::vector<std::tuple<uint16_t, uint16_t, uint16_t>> &encode) {
    std::pair<uint16_t, uint16_t> encodeData{};
    const auto dataSize{ data.size() };
    encode.reserve(dataSize);

    for (std::size_t i{}; i < dataSize; i++) {
        std::size_t j { i };
        int count{ 1 };
        const auto current { data[i] };

        while (j < dataSize - 1 && current == data[j+1]) {
            count++;
            j++;
        }
        i = j;

        encodeData.first = count;
        encodeData.second = current;

        // if (!encode.empty() && encode.back() == encodeData)
        //     continue;

        // encode.push_back(encodeData);
    }
}

void rleEncode(const std::vector<int> &data, std::vector<RLE_JEW> &encode) {
    RLE_JEW encodeData{};
    const auto dataSize{ data.size() };
    encode.reserve(dataSize);
    int repeatCount{ 1 };

    for (std::size_t i{}; i < dataSize; i++) {
        std::size_t j { i };
        int count{ 1 };
        const auto current { data[i] };

        while (j < dataSize - 1 && current == data[j+1]) {
            count++;
            j++;
        }

        if (!encode.empty() && encode.back().count == count && encode.back().grayVal == current) {
            encode.back().repeat++;
            break;
        } else {
            repeatCount = 1;
        }

        i = j;

        // encode.push_back(encodeData);
    }
}