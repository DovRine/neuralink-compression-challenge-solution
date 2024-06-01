import wave
import numpy as np
from typing import List, Tuple


class WavFile(object):
    """ Class for reading, storing, reconstructing, and compressing wav files.
    """
    def __init__(self, filename: str):
        self.filename: str = filename
        self.params: Tuple[int, int, int, int, str, str]
        self.frames: bytes
        self.samples: np.ndarray
        self.bitarray: List[int] = []

        self.__read_wav_file()

    def __read_wav_file(self):
        """ Read a WAV file and extract parameters, frames and other data.

        Returns:
            None

        """
        with wave.open(self.filename, 'rb') as wav_file:
            self.params = wav_file.getparams()
            self.frames = wav_file.readframes(wav_file.getnframes())
            self.samples = np.frombuffer(self.frames, dtype=np.int16)
            self.bitarray = np.unpackbits(self.samples.view(np.uint8), bitorder='little').tolist()

    @staticmethod
    def decompress_tuple(bit_tuple: Tuple[int, int]) -> List[int]:
        """ Decompress a tuple of bits.

        Args:
            bit_tuple (Tuple[int, int]): A tuple of bits.

        Returns:
            List[int]: A list of bits.
        """
        return [0] * bit_tuple[0] + [1] * bit_tuple[1]

    def compress_bitarray_to_tuples(self, bitarray: List[int] = None) -> List[Tuple[int, int]]:
        """ Compress a list of bits using indexed run-length encoding.

        Args:
            bitarray (List[int]): The list of bits to compress.

        Returns:
            List[Tuple[int, int]]: A list of tuples representing consecutive counts of 0's and 1's.
        """
        if not bitarray:
            bitarray = self.bitarray

        compressed_array = []
        current_index = 0
        total_length = len(bitarray)
        collected = []

        while current_index < total_length:
            processed_count = 0
            for index in range(current_index, total_length):
                # Check to see if the current bit is 0 and the previous is 1 (leads to the counting stopping)
                if bitarray[index] == 0 and bitarray[index - 1] == 1 and processed_count > 0:
                    break

                collected.append(bitarray[index])
                processed_count += 1

            compressed_array.append((collected.count(0), collected.count(1)))
            current_index += processed_count

            # Reset the collected list (VERY IMPORTANT)
            collected = []

        return compressed_array

    def bitarray_to_frames(self):
        """ Converts a bitarray to audio frames losslessly

        Returns:
            ndarray
        """
        # Convert bit array back to a numpy array of 8-bit unsigned integers
        np_array = np.array(self.bitarray, dtype=np.uint8)
        repacked_bits = np.packbits(np_array, bitorder='little')

        # Interpret the bytes as 16-bit signed integers
        reconstructed_samples = repacked_bits.view(np.int16)

        # Convert the samples back to byte frames
        reconstructed_frames = reconstructed_samples.tobytes()

        return reconstructed_frames

    def reconstruct_file_from_bitarray(self, filename: str = '', bitarray=None):
        """

        Args:
            filename (str): Name of file (and path) for the new file to save
            bitarray:

        Returns:

        """
        if not filename:
            filename = 'bitarray_reconstructed_audio.wav'

        if not bitarray:
            bitarray = self.bitarray

        packed_bits = np.packbits(np.array(bitarray, dtype=np.uint8), axis=-1, bitorder='little')
        sample_frames = packed_bits.view(np.int16).tobytes()
        self.__write_file_from_frames(filename=filename, frames=sample_frames)

    def __write_file_from_frames(self, filename: str = '', frames: bytes = bytes()):
        """ Write audio frames to a WAV file with given parameters.

        Args:
            filename (str): The path to the file [optional]
            frames (bytes): array of bytes as wav frames [optional]

        Returns:
            None
        """
        if not filename:
            filename = self.filename

        if not frames:
            frames = self.frames

        with wave.open(filename, 'wb') as output_wav:
            output_wav.setparams(self.params)
            output_wav.writeframes(frames)