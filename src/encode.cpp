#include "encode.h"

std::vector<std::pair<int, int>> indexedRunLengthEncode(const std::vector<int>& data) {
    std::vector<std::pair<int, int>> encodedData;
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == 1) {
            int startIndex = i;
            int count = 0;
            while (i < data.size() && data[i] == 1) {
                count++;
                i++;
            }
            encodedData.push_back({startIndex, count});
        }
    }
    return encodedData;
}

std::vector<int> indexedRunLengthDecode(const std::vector<std::pair<int, int>>& encodedData, size_t dataSize) {
    std::vector<int> decodedData(dataSize, 0);
    for (const auto& [startIndex, count] : encodedData) {
        for (size_t i = 0; i < count; ++i) {
            decodedData[startIndex + i] = 1;
        }
    }
    return decodedData;
}
