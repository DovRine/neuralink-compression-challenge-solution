#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "encode.h"

// Define the chunk size in bytes (1 second of audio data)
const int CHUNK_SIZE = 39062;
const int THRESHOLD = 1000; // Threshold to convert audio samples to binary values

// Function to read data from a .wav file
std::vector<int16_t> readWavFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Could not open file: " << filename << std::endl;
        exit(1);
    }

    // Skip the header and read the data
    file.seekg(44); // Skip the WAV header
    std::vector<int16_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Convert the byte data to int16_t
    int dataSize = data.size() / 2;
    std::vector<int16_t> audioData(dataSize);
    memcpy(audioData.data(), data.data(), dataSize * sizeof(int16_t));

    return audioData;
}

// DJB2 hash function
unsigned long hash(const std::string& str) {
    unsigned long hash = 5381;
    int c;
    const char* s = str.c_str();

    while ((c = *s++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}

// Function to convert a hash value to a hexadecimal string
std::string hashToHexString(unsigned long hash) {
    std::ostringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << hash;
    return ss.str();
}

// Function to process audio data in chunks and generate histograms and hashes
void processAudioChunks(const std::vector<int16_t>& audioData, int chunkSize, int bins, std::vector<std::vector<int>>& histograms, std::vector<std::string>& hashes) {
    int nChunks = audioData.size() / (chunkSize / 2);  // Each sample is 2 bytes
    int binSize = 65536 / bins;

    for (int i = 0; i < nChunks; ++i) {
        int start = i * (chunkSize / 2);
        int end = start + (chunkSize / 2);
        std::vector<int16_t> chunk(audioData.begin() + start, audioData.begin() + end);

        // Generate histogram
        std::vector<int> hist(bins, 0);
        for (int16_t sample : chunk) {
            int bin = (sample + 32768) / binSize;
            hist[bin]++;
        }
        histograms.push_back(hist);

        // Hash the histogram
        std::ostringstream histStream;
        for (int count : hist) {
            histStream << count;
        }

        unsigned long hashValue = hash(histStream.str());
        std::ostringstream hashStream;
        hashStream << std::hex << std::setw(8) << std::setfill('0') << hashValue;

        hashes.push_back(hashStream.str());
    }
}

// Function to write encoded data, histograms, and hashes to a file
void writeEncodedData(const std::string& filename, const std::vector<std::pair<int, int>>& encodedData, const std::vector<std::vector<int>>& histograms, const std::vector<std::string>& hashes) {
    std::ofstream file(filename);
    for (const auto& [startIndex, count] : encodedData) {
        file << startIndex << " " << count << "\n";
    }

    for (size_t i = 0; i < histograms.size(); ++i) {
        file << "Histogram " << i << ": ";
        for (int bin : histograms[i]) {
            file << bin << " ";
        }
        file << "\nHash " << i << ": " << hashes[i] << "\n";
    }
}

// Test function for encoding and decoding a single slice
void testSingleSlice(const std::vector<int16_t>& audioData, int chunkSize) {
    std::vector<int16_t> slice(audioData.begin(), audioData.begin() + chunkSize / 2);

    // Convert to binary values using the threshold
    std::vector<int> binarySlice(slice.size());
    for (size_t i = 0; i < slice.size(); ++i) {
        binarySlice[i] = (abs(slice[i]) > THRESHOLD) ? 1 : 0;
    }

    // Encode the binary slice
    std::vector<std::pair<int, int>> encodedData = indexedRunLengthEncode(binarySlice);

    // Print the encoded data
    std::cout << "Encoded Data: ";
    for (const auto& [startIndex, count] : encodedData) {
        std::cout << "(" << startIndex << ", " << count << ") ";
    }
    std::cout << std::endl;

    // Decode the encoded data
    std::vector<int> decodedData = indexedRunLengthDecode(encodedData, binarySlice.size());

    // Verify if the decoded data matches the original binary slice
    bool isMatch = (binarySlice == decodedData);
    std::cout << "Decoded Data Matches: " << (isMatch ? "Yes" : "No") << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: encode <input_file> <output_file> [tests]" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    bool runTests = (argc == 4 && std::string(argv[3]) == "tests");

    std::vector<int16_t> audioData = readWavFile(inputFile);

    if (runTests) {
        // Perform a test with a single slice
        std::cout << "Running single slice test..." << std::endl;
        testSingleSlice(audioData, CHUNK_SIZE);
    } else {
        // Process the audio data in chunks
        std::vector<std::vector<int>> histograms;
        std::vector<std::string> hashes;
        processAudioChunks(audioData, CHUNK_SIZE, 256, histograms, hashes);

        // Compress the original data using Indexed Run-Length Encoding
        std::vector<int> binaryData(audioData.size());
        for (size_t i = 0; i < audioData.size(); ++i) {
            binaryData[i] = (audioData[i] != 0) ? 1 : 0;
        }
        std::vector<std::pair<int, int>> encodedData = indexedRunLengthEncode(binaryData);

        // Write the encoded data, histograms, and hashes to the output file
        writeEncodedData(outputFile, encodedData, histograms, hashes);
    }

    return 0;
}
