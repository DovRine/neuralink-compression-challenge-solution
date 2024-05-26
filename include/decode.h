#ifndef DECODE_H
#define DECODE_H

#include <vector>
#include <utility>
#include <cstddef> // Include this header for size_t

std::vector<int> indexedRunLengthDecode(const std::vector<std::pair<int, int>>& encodedData, size_t dataSize);

#endif // DECODE_H
