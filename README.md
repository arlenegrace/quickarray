Work in progress.
## quick_array
`quick_array` is a high performance resizable array library written in C++. Features include:

- **High performance**. This library was built from the ground up with performance in mind. Initial benchmarks outperform GCC's implementation of `std::vector` by a factor of six.

- **Simple integration**. The entire library consists of a single header file. The code is written in vanilla C++ that has been tested on GCC and MSVC.

- **Memory efficient**. `quick_array` employs small object optimization by allocating the underlying array on the stack to save memory when storing up to 23 bytes of data on the x86_64 platform.

### Todo:
- Documentation
- Possible C implementation
