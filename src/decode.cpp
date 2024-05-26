#include "decode.h"

std::vector<int> indexedRunLengthDecode(const std::vector<std::pair<int, int>>& encodedData, size_t dataSize) {
    std::vector<int> decodedData(dataSize, 0); // Initialize with default values

    for (const auto& [startIndex, count] : encodedData) {
        for (int i = 0; i < count; ++i) {
            decodedData[startIndex + i] = 1;
        }
    }

    return decodedData;
}
