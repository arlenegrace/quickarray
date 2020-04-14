


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


#include <stdexcept> 
#include <utility> /* std::swap, std::initializer_list, std::memcpy */


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
    constexpr vec_state vec_state() const noexcept;
    constexpr bool      is_heap() const noexcept;
    constexpr bool      is_stack() const noexcept;
    constexpr size_t&   get_size() const noexcept;
              void      heap_init(size_t size, size_t cap);
              void      stack_init(size_t size) noexcept;
              void      stack_to_heap(size_t arr_size);
              void      heap_resize(size_t reserve_size);
    static    T&        back_impl(size_t last, T* buff);
    static    T&        at_impl(size_t index, size_t arr_size, T* buff);
              void      push_back_impl(size_t& size, T* buff, T item);
              void      quick_array_init(const std::initializer_list<T>& arr, size_t size);

public:
    /* CONSTRUCTION AND DESTRUCTION */
    quick_array() noexcept;
    quick_array(size_t cap);
    quick_array(std::initializer_list<T> arr);
    quick_array(std::initializer_list<T> arr, size_t size);
    quick_array(const quick_array<T>& other); /* Not tested */
    quick_array(quick_array<T>&& other) noexcept; /* Not tested */
    ~quick_array() noexcept;

    quick_array<T>& operator=(quick_array<T> other) noexcept; /* Not tested */
    
    /* MODIFIER METHODS */
    void clear() noexcept;
    void insert(size_t index, T& element);
    void push_back(T item);
    void emplace_back(); /* Not defined */
    void pop_back() noexcept;
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

template <typename T>
constexpr enum class quick_array<T>::vec_state quick_array<T>::vec_state() const noexcept
{
    return state;
}

template <typename T>
constexpr bool quick_array<T>::is_heap() const noexcept
{
    return state == vec_state::heap;
}

template <typename T>
constexpr bool quick_array<T>::is_stack() const noexcept
{
    return state == vec_state::stack;
}

template <typename T>
void quick_array<T>::heap_init(size_t size, size_t cap)
{
    state = vec_state::heap;
    hvec.size = size;
    hvec.capacity = cap;
    hvec.array = new T[cap];
}

template <typename T>
void quick_array<T>::stack_init(size_t size) noexcept
{
    state = vec_state::stack;
    svec.size = size;
}

template <typename T>
void quick_array<T>::stack_to_heap(size_t arr_size)
{
    size_t stack_size = svec.size;
    T* array_temp = new T[arr_size];
    std::memcpy(array_temp, svec.array, stack_bytes<T>);
    state = vec_state::heap;
    hvec.size = stack_size;
    hvec.capacity = arr_size;
    hvec.array = array_temp;
}

template <typename T>
void quick_array<T>::heap_resize(size_t reserve_size)
{
    T* array_temp = new T[reserve_size];
    std::memcpy(array_temp, hvec.array, sizeof(T) * hvec.size);
    delete[] hvec.array;
    hvec.array = array_temp;
    hvec.capacity = reserve_size;
}

template <typename T>
T& quick_array<T>::at_impl(size_t index, size_t arr_size, T* buff)
{
    if (index >= arr_size)
        throw std::out_of_range("Index is out of bounds.");
    else
        return buff[index];
}

template <typename T>
T& quick_array<T>::back_impl(size_t last, T* buff)
{
    return buff[last - 1];
}

template <typename T>
void quick_array<T>::push_back_impl(size_t& size, T* buff, T item)
{
    buff[size] = item;
    size++;
}

template <typename T>
void quick_array<T>::quick_array_init(const std::initializer_list<T>& arr, size_t cap)
{
    if (cap * sizeof(T) > stack_bytes<T>) {
        heap_init(arr.size(), cap);
        std::memcpy(hvec.array, arr.begin(), sizeof(T) * arr.size());
    } else {
        stack_init(arr.size());
        std::memcpy(svec.array, arr.begin(), sizeof(T) * arr.size());
    }
}

template <typename T>
quick_array<T>::quick_array() noexcept
{
    svec.size = 0;
}

template <typename T>
quick_array<T>::quick_array(size_t cap)
{
    is_stack() ? stack_init() : heap_init(0, cap);
}

template <typename T>
quick_array<T>::quick_array(std::initializer_list<T> arr)
{
    quick_array_init(arr, arr.size());
}

template <typename T>
quick_array<T>::quick_array(std::initializer_list<T> arr, size_t size)
{
    assert(arr.size() >= size);

    quick_array_init(arr, size);
}

template <typename T>
quick_array<T>::quick_array(const quick_array<T>& other)
{
    if (other.is_heap()) {
        heap_init(other.hvec.size, other.hvec.capacity);
        std::memcpy(hvec.array, other.hvec.array, sizeof(T) * other.hvec.size);
    } else {
        stack_init(other.svec.size);
        std::memcpy(svec.array, other.svec.array, sizeof(T) * other.svec.size);
    }
}

template <typename T>
quick_array<T>::quick_array(quick_array<T>&& other) noexcept
{
    hvec = other.hvec;
    state = other.state;
}

template <typename T>
quick_array<T>::~quick_array() noexcept
{
    if (is_heap())
        delete[] hvec.array;
}

template <typename T>
quick_array<T>& quick_array<T>::operator=(quick_array<T> other) noexcept
{
    swap(other);
    return *this;
}

template <typename T>
void quick_array<T>::clear() noexcept
{
    is_heap() ? hvec.size = 0 : svec.size = 0;
}

template <typename T>
void quick_array<T>::insert(size_t index, T& element)
{
    this[index] = element;
}

template <typename T>
void quick_array<T>::push_back(T item)
{
    if (is_heap()) {
        if (hvec.size == hvec.capacity)
            heap_resize(hvec.capacity * QA_GROWTH_FACTOR);
        push_back_impl(hvec.size, hvec.array, item);
    } else if (svec.size == svec.capacity) {
        stack_to_heap(svec.capacity * QA_GROWTH_FACTOR);
        push_back_impl(hvec.size, hvec.array, item);
    } else {
        push_back_impl(svec.size, svec.array, item);
    }
}

template <typename T>
void quick_array<T>::pop_back() noexcept
{
    get_size() -= 1;
}

template <typename T>
void quick_array<T>::swap(quick_array<T>& other) noexcept
{
    std::swap(hvec, other.hvec);
    std::swap(state, other.state);
}

template <typename T>
void swap(quick_array<T>& first, quick_array<T>& second) noexcept
{
    std::swap(first.hvec, second.hvec);
    std::swap(first.state, second.state);
}

template <typename T>
constexpr bool quick_array<T>::empty() const noexcept
{
    return get_size() == 0;
}

template <typename T>
constexpr size_t& quick_array<T>::get_size() const noexcept
{
    return is_heap() ? hvec.size : svec.size;
}

template <typename T>
constexpr size_t quick_array<T>::size() const noexcept
{
    return get_size();
}

template <typename T>
constexpr size_t quick_array<T>::max_size() noexcept
{
    return size_t{-1} / sizeof(T);
}

template <typename T>
constexpr size_t quick_array<T>::capacity() const noexcept
{
    return is_heap() ? hvec.capacity : svec.capacity;
}

template <typename T>
void quick_array<T>::reserve(size_t reserve_size)
{
    if (!is_heap())
        stack_to_heap(reserve_size);
    if (reserve_size > hvec.capacity)
        heap_resize(reserve_size);
}

template <typename T>
void quick_array<T>::shrink_to_fit() noexcept
{
    if (is_heap())
        heap_resize(hvec.size);
}

template <typename T>
auto quick_array<T>::begin() noexcept
{
    return data();
}

template <typename T>
auto quick_array<T>::end() noexcept
{
    return is_heap() ? &back_impl(hvec.size + 1, hvec.array) : &back_impl(svec.size + 1, svec.array);
}

template <typename T>
const auto quick_array<T>::begin_c() const noexcept
{
    return begin();
}

template <typename T>
const auto quick_array<T>::end_c() const noexcept
{
    return end();
}

template <typename T>
T& quick_array<T>::at(size_t index) noexcept
{
    return is_heap() ? at_impl(index, hvec.size, hvec.array) : at_impl(index, svec.size, svec.array);
}

template <typename T>
const T& quick_array<T>::at_c(size_t index) const noexcept
{
    return at(index);
}

template <typename T>
T& quick_array<T>::operator[](size_t counter)
{
    return data()[counter];
}

template <typename T>
T& quick_array<T>::front() noexcept
{
    return at(0);
}

template <typename T>
const T& quick_array<T>::front_c() const noexcept
{
    return front();
}

template <typename T>
T& quick_array<T>::back() noexcept
{
    return is_heap() ? back_impl(hvec.size, hvec.array) : back_impl(svec.size, svec.array);
}

template <typename T>
const T& quick_array<T>::back_c() const noexcept
{
    return back();
}

template <typename T>
T* quick_array<T>::data() noexcept
{
    return is_heap() ? hvec.array : svec.array;
}

template <typename T>
const T* quick_array<T>::data_c() const noexcept
{
    return data();
}

#endif /* QUICK_ARRAY_560FDBD65BB6 */