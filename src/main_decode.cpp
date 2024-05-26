#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint> // Include this header for int16_t and int32_t
#include "decode.h"

std::vector<std::pair<int, int>> readEncodedData(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::pair<int, int>> encodedData;
    int startIndex, count;
    while (file >> startIndex >> count) {
        encodedData.push_back({startIndex, count});
    }
    return encodedData;
}

void writeWavFile(const std::string& filename, const std::vector<int>& data, int sampleRate, int bitsPerSample, int numChannels) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        exit(1);
    }

    // Write WAV file header
    file.write("RIFF", 4);
    int32_t chunkSize = 36 + data.size() * (bitsPerSample / 8);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    int32_t subchunk1Size = 16;
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    int16_t audioFormat = 1; // PCM
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    int32_t byteRate = sampleRate * numChannels * (bitsPerSample / 8);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    int16_t blockAlign = numChannels * (bitsPerSample / 8);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    int32_t subchunk2Size = data.size() * (bitsPerSample / 8);
    file.write(reinterpret_cast<const char*>(&subchunk2Size), 4);

    // Convert binary format (0s and 1s) back to original format
    std::vector<int16_t> outputData(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        outputData[i] = (data[i] == 1) ? 32767 : 0;
    }

    file.write(reinterpret_cast<const char*>(outputData.data()), outputData.size() * sizeof(int16_t));
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Usage: decode <input_file> <output_file> <data_size> <sample_rate> <bits_per_sample>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    size_t dataSize = std::stoul(argv[3]);
    int sampleRate = std::stoi(argv[4]);
    int bitsPerSample = std::stoi(argv[5]);
    int numChannels = 1; // Assuming mono-channel for simplicity

    std::vector<std::pair<int, int>> encodedData = readEncodedData(inputFile);
    std::vector<int> decodedData = indexedRunLengthDecode(encodedData, dataSize);

    writeWavFile(outputFile, decodedData, sampleRate, bitsPerSample, numChannels);

    return 0;
}
