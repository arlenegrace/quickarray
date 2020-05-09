## quick_array
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

`quick_array` is a fast resizable array library written in C++ with a large focus on performance, maintainability and code readability. It aims to be a drop-in replacement to std::vector with additional features.

### Features
- **High performance**. This library was built from the ground up with performance in mind. Initial benchmarks outperform GCC's implementation of `std::vector` by a factor of six.
- **Simple integration**. The entire library consists of a single header file. The code is written in vanilla C++ that has been tested on GCC and MSVC.
- **Memory efficient**. `quick_array` employs small object optimization by allocating the underlying array on the stack to save memory when storing up to 23 bytes of data on the x86_64 platform.
- **Configurable**. Easily control settings such as the growth multiplier to optimize performance for individual programs.

### To-do
- Documentation
- C implementation

### License
This project is open source and available under the [MIT License](LICENSE).
