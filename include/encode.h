#ifndef ENCODE_H
#define ENCODE_H

#include <vector>
#include <utility>
#include <cstddef> // Include this header for size_t

std::vector<std::pair<int, int>> indexedRunLengthEncode(const std::vector<int>& data);
std::vector<int> indexedRunLengthDecode(const std::vector<std::pair<int, int>>& encodedData, size_t dataSize);

#endif // ENCODE_H
