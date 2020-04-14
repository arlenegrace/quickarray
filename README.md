Work in progress.
## quick_array
`quick_array` is a high performance resizable array library written in C++. Features include:

- **High performance**. This library was built from the ground up with performance in mind. Initial benchmarks outperform GCC's implementation of `std::vector` by a factor of six.

- **Simple integration**. The entire library consists of a single header and source file (to be merged). The code is written in vanilla C++ that has been tested on GCC and MSVC.

- **Memory efficient**. `quick_array` allocates the underlying array on the stack to save memory when storing less than 23 bytes of data. 

### Todo:
- Documentation
- Merge source and header file for even simpler usage
- Possible C implementation
