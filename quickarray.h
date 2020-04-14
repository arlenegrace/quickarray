


#ifndef QUICK_ARRAY_560FDBD65BB6
#define QUICK_ARRAY_560FDBD65BB6

#define DEBUG false
#if DEBUG == true
    #include <iostream>

    void* operator new(size_t size)
    {
        std::cout << "=========== Allocated " << size << " bytes\n";
        return malloc(size);
    }
    void operator delete(void* memory)
    {
        std::cout << "========== Freed \n";
        free(memory);
    }
    #define LOG_DEBUG(x) std::cout << x
#else
    #define LOG_DEBUG(x)
#endif

#ifndef QA_GROWTH_FACTOR
#define QA_GROWTH_FACTOR (2)
#endif


#include <cstring>
#include <stdexcept> 
#include <utility>


/* DECLARATIONS */
template <class T> class quick_array;
template <class T> class heap_array;
template <class T> class stack_array;


/* CLASSES */
template <typename T>
class heap_array
{
    friend class quick_array<T>;
    size_t size;
    size_t capacity;
    T* array;
};


template <typename T>
constexpr auto stack_bytes = size_t{ sizeof(heap_array<T>) - 1 };


template <class T>
class stack_array
{
    friend class quick_array<T>;
    static constexpr auto capacity = size_t{ stack_bytes<T> / sizeof(T) };
    T array[capacity];
    uint8_t size;
};


template <class T>
class quick_array
{
    union {
        heap_array<T> hvec;
        stack_array<T> svec;
    };
    
    enum class vec_state : uint8_t { stack, heap };
    vec_state state = vec_state::stack;
    
    /* PRIVATE HELPER METHODS */
    inline constexpr vec_state vec_state() const noexcept;
    inline constexpr bool      is_heap() const noexcept;
    inline constexpr bool      is_stack() const noexcept;
    inline           void      heap_init(size_t size, size_t cap);
    inline           void      stack_init(size_t size) noexcept;
    inline           void      stack_to_heap(size_t arr_size);
    inline           void      heap_enlarge(size_t reserve_size);
    inline static    T&        back_impl(size_t last, T* buff);
    inline static    T&        at_impl(size_t index, size_t arr_size, T* buff);
    inline           void      push_back_impl(size_t& size, T* buff, T item);
    inline           void      quick_array_init(const std::initializer_list<T>& arr, size_t size);

public:
    /* CONSTRUCTION AND DESTRUCTION */
    quick_array() noexcept;
    quick_array(std::initializer_list<T> arr);
    quick_array(std::initializer_list<T> arr, size_t size);
    quick_array(const quick_array<T>& other); /* Not tested */
    quick_array(quick_array<T>&& other); /* Not tested */
    ~quick_array();

    quick_array<T>& operator=(quick_array<T> other); /* Not tested */
    
    /* MODIFIER METHODS */
    void clear() noexcept;
    void insert(size_t index, T& element);
    void push_back(T item);
    void emplace_back(); /* Not defined */
    void pop_back();
    void swap(quick_array<T>& other) noexcept;
    friend void swap(quick_array<T>& first, quick_array<T>& second) noexcept;
    
    /* CAPACITY METHODS */
    constexpr        bool   empty() const noexcept;
    constexpr        size_t size() const noexcept;
    constexpr static size_t max_size() noexcept;
    constexpr        size_t capacity() const noexcept;
                     void   reserve(size_t reserve_size);
                     void   shrink_to_fit() noexcept;
    
    /* ITERATOR METHODS */
    auto begin() noexcept;
    auto end() noexcept;
    const auto begin_c() const noexcept; /* Not tested */
    const auto end_c() const noexcept; /* Not tested */
    
    /* ELEMENT ACCESS METHODS */
          T& at(size_t index) noexcept;
    const T& at_c(size_t index) const noexcept; /* Not tested */
          T& operator[](size_t counter);
          T& front() noexcept;
    const T& front_c() const noexcept; /* Not tested */
          T& back() noexcept;
    const T& back_c() const noexcept; /* Not tested */
          T* data() noexcept;
    const T* data_c() const noexcept; /* Not tested */
};


#include "quickarray.cpp"

#endif /* QUICK_ARRAY_560FDBD65BB6 */