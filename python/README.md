# Python Directory Details
The python in this project is not necessarily meant to be run, but it can be.

The python code in this project is maintained for archival purposes and to more quickly prototype testable compression algorithms.

## Requirements(.txt)
Package requirements for the existing functionality are maintained in the accompanying `requirements.txt` file (for use with `pip install`).

## Experiments (Entrypoint File)
The file `experiments.py` can be thought of an entry point for testing implementations, or at least that's how I do it.

Stable implementations of useful functions or classes get moved out of `experiments.py` and into their new homes to keep this file clean and easy to work with.

## Utilities
There is a `/utilities` directory which can be imported from directly.

There are a handful of pure utility functions defined within `__init__.py` that can be imported anywhere.

The WavFile class is a wrapper around more complex operations for handling wav files for the purposes of reducing code replication for basic read/write/analysis operations.

## Hack the Planet!