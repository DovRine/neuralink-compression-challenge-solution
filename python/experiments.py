""" ./main.py """
import sys

from utilities import WavFile, flatten_tuple_array, count_all_unique_tuples_in_data


if __name__ == '__main__':
    # Count all unique tuples in all audio files in /data
    # all_unique_tuples = count_all_unique_tuples_in_data()


    wav_file_001 = WavFile('data/00d4f842-fc92-45f5-8cae-3effdc2245f5.wav')

    size_frames = sys.getsizeof(wav_file_001.frames)
    size_samples = wav_file_001.samples.nbytes

    bitarray_size = sys.getsizeof(wav_file_001.bitarray)
    bitarray_len = len(wav_file_001.bitarray)

    tuple_array = wav_file_001.compress_bitarray_to_tuples()
    tuple_array_size = sys.getsizeof(tuple_array)
    tuple_array_len = len(tuple_array)

    flattened_tuple_array = flatten_tuple_array(tuple_array)
    flattened_tuple_array_size = sys.getsizeof(flattened_tuple_array)

    breakpoint()
