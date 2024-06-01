""" This module is the entry point to the modules of the utilities package and other tools """
import hashlib
import os

from typing import List, Tuple

from .WavFile import WavFile


def md5sum(filename: str) -> str:
    """ Calculate the MD5 checksum of a file.

    Args:
        filename (str): The path to the file.

    Returns:
        str: The MD5 checksum of the file.
    """
    with open(filename, 'rb') as f:
        file_hash = hashlib.md5()
        while chunk := f.read(8192):
            file_hash.update(chunk)
    return file_hash.hexdigest()


def count_unique_tuples(tuple_array: List[Tuple[int, int]]) -> dict:
    """ Count the number of unique tuples in a list.

    Args:
        tuple_array (List[Tuple[int, int]]): A list of tuples.

    Returns:
        dict: A dictionary of unique tuples and their counts.
    """
    tuple_counts = {}

    for compressed_tuple in tuple_array:
        # Check if the tuple is already in the dictionary
        if compressed_tuple in tuple_counts:
            # If it is, increment the count by 1
            tuple_counts[compressed_tuple] += 1
        else:
            # If it is not, add it to the dictionary with a count of 1
            tuple_counts[compressed_tuple] = 1

    return tuple_counts


def count_all_unique_tuples_in_data():
    all_unique_tuples = {}

    # Create a list of all file names within the directory /data
    file_names = ['data/' + file for file in os.listdir('data/') if file.endswith('.wav')]
    file_count = len(file_names)
    print(f'Files to be processed: {file_count}')

    for file_name in file_names:
        print(f"Processing {file_name}: Position {file_names.index(file_name) + 1} of {file_count}")
        wav_file = WavFile(file_name)
        wav_tuples = wav_file.compress_bitarray_to_tuples()
        unique_tuples = count_unique_tuples(wav_tuples)

        print(f"{file_name}: Unique Tuples Count {len(unique_tuples)}")
        all_unique_tuples = {**all_unique_tuples, **unique_tuples}

        print(f"Current Unique Tuples Count: {len(all_unique_tuples)}")

    print(f"Unique Tuples Count: {len(all_unique_tuples)}")
    print(f"Unique Tuples: {all_unique_tuples}")
    return all_unique_tuples


def flatten_tuple_array(tuple_array: List[Tuple[int, int]]) -> List[int]:
    """ Flatten an array of tuples.

    Args:
        tuple_array:

    Returns:

    """
    # Initialize an empty list to store the flattened elements
    flattened_list = []

    # Iterate through each tuple in the input array
    for tpl in tuple_array:
        # Iterate through each element in the tuple
        for element in tpl:
            # Append each element to the flattened list
            flattened_list.append(element)

    return flattened_list

