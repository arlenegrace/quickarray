

/*
 * ===============================================================
 *
 *                  CONSTRUCTION & DESTRUCTION
 *
 * ===============================================================
 * 
 *    quick_array() noexcept;
 *    quick_array(size_t cap);
 *    quick_array(std::initializer_list<T> arr);
 *    quick_array(std::initializer_list<T> arr, size_t size);
 *    quick_array(const quick_array<T>& other);
 *    quick_array(quick_array<T>&& other) noexcept;
 *    ~quick_array() noexcept;
 *    quick_array<T>& operator=(quick_array<T> other) noexcept;
 *
 *
 * ===============================================================
 *
 *                          MODIFIERS
 *
 * ===============================================================
 * 
 *           void clear() noexcept;
 *           void insert(size_t index, T& element);
 *           void push_back(T item);
 *           void emplace_back();
 *           void pop_back() noexcept;
 *           void swap(quick_array<T>& other) noexcept;
 *    friend void swap(quick_array<T>& first, quick_array<T>& second) noexcept;
 *
 *
 * ===============================================================
 *
 *                          CAPACITY
 *
 * ===============================================================
 *
 *    constexpr        bool   empty() const noexcept;
 *    constexpr        size_t size() const noexcept;
 *    constexpr static size_t max_size() noexcept;
 *    constexpr        size_t capacity() const noexcept;
 *                     void   reserve(size_t reserve_size);
 *                     void   shrink_to_fit() noexcept;
 *
 *
 * ===============================================================
 *
 *                          ITERATORS
 *
 * ===============================================================
 *
 *          auto begin() noexcept;
 *          auto end() noexcept;
 *    const auto begin_c() const noexcept;
 *    const auto end_c() const noexcept;
 *
 *
 * ===============================================================
 *
 *                          ELEMENT ACCESS
 *
 * ===============================================================
 *
 *          T& at(size_t index) noexcept;
 *    const T& at_c(size_t index) const noexcept;
 *          T& operator[](size_t counter);
 *          T& front() noexcept;
 *    const T& front_c() const noexcept;
 *          T& back() noexcept;
 *    const T& back_c() const noexcept;
 *          T* data() noexcept;
 *    const T* data_c() const noexcept;
 */


#ifndef QUICK_ARRAY_560FDBD65BB6
#define QUICK_ARRAY_560FDBD65BB6

#include <stdexcept> /* std::out_of_range */
#include <utility> /* std::swap, std::initializer_list, std::memcpy */

#define DEBUG false
#if DEBUG == true
#   include <iostream>

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
#   define LOG_DEBUG(x) std::cout << x
#else
#   define LOG_DEBUG(x)
#endif

#ifndef QA_GROWTH_FACTOR
#define QA_GROWTH_FACTOR (2)
#endif

#ifndef QA_API
#define QA_API inline
#endif



/*
 * ===============================================================
 *
 *                          DECLARATIONS
 *
 * ===============================================================
 */

template <class T> class quick_array;
template <class T> class heap_array;
template <class T> class stack_array;


template <typename T>
constexpr auto QA_STACK_CAPACITY = size_t{ sizeof(heap_array<T>) - 1 };

template <typename T>
class heap_array
{
    friend class quick_array<T>;

    T* array;
    size_t size;
    size_t capacity;
};

template <class T>
class stack_array
{
    friend class quick_array<T>;
    static constexpr auto capacity = size_t{ QA_STACK_CAPACITY<T> / sizeof(T) };

    T array[capacity];
    uint8_t size;
};

template <class T>
class quick_array
{
    union {
        heap_array<T> heap;
        stack_array<T> stack;
    };
    
    enum class array_state : uint8_t { stack, heap };
    array_state state = array_state::stack;
    
    /* Private helpers */
    constexpr array_state array_state() const noexcept;
    constexpr bool     is_heap() const noexcept;
    constexpr bool     is_stack() const noexcept;
              void     heap_init(size_t size, size_t cap);
              void     stack_init(size_t size) noexcept;
              void     stack_to_heap(size_t arr_size);
              void     heap_resize(size_t reserve_size);
    static    T&       back_impl(size_t last, T* buff);
    static    T&       at_impl(size_t index, size_t arr_size, T* buff);
              void     push_back_impl(size_t& size, T* buff, T item);
              void     quick_array_init(const std::initializer_list<T>& arr, size_t size);

public:
    quick_array() noexcept;
    quick_array(size_t cap);
    quick_array(std::initializer_list<T> arr);
    quick_array(std::initializer_list<T> arr, size_t size);
    quick_array(const quick_array<T>& other); /* Not tested */
    quick_array(quick_array<T>&& other) noexcept; /* Not tested */
    ~quick_array() noexcept;

    quick_array<T>& operator=(quick_array<T> other) noexcept; /* Not tested */
    
    /* Modifiers */
           void clear() noexcept;
           void insert(size_t index, T& element);
           void push_back(T item);
           void emplace_back(); /* Not defined */
           void pop_back() noexcept;
           void swap(quick_array<T>& other) noexcept;
    friend void swap(quick_array<T>& first, quick_array<T>& second) noexcept;
    
    /* Capacity */
    constexpr        bool   empty() const noexcept;
    constexpr        size_t size() const noexcept;
    constexpr static size_t max_size() noexcept;
    constexpr        size_t capacity() const noexcept;
                     void   reserve(size_t reserve_size);
                     void   shrink_to_fit() noexcept;
    
    /* Iterators */
          auto begin() noexcept;
          auto end() noexcept;
    const auto begin_c() const noexcept; /* Not tested */
    const auto end_c() const noexcept; /* Not tested */
    
    /* Element access */
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


/*
 * ===============================================================
 *
 *                         PRIVATE HELPERS
 *
 * ===============================================================
 */

template <typename T>
QA_API constexpr enum class quick_array<T>::array_state quick_array<T>::array_state() const noexcept
{
    return state;
}

template <typename T>
QA_API constexpr bool quick_array<T>::is_heap() const noexcept
{
    return state == array_state::heap;
}

template <typename T>
QA_API constexpr bool quick_array<T>::is_stack() const noexcept
{
    return state == array_state::stack;
}

template <typename T>
QA_API void quick_array<T>::heap_init(size_t size, size_t cap)
{
    state = array_state::heap;
    heap.size = size;
    heap.capacity = cap;
    heap.array = new T[cap];
}

template <typename T>
QA_API void quick_array<T>::stack_init(size_t size) noexcept
{
    state = array_state::stack;
    stack.size = size;
}

template <typename T>
QA_API void quick_array<T>::stack_to_heap(size_t arr_size)
{
    const size_t stack_size = stack.size;
    T* array_temp = new T[arr_size];
    std::memcpy(array_temp, stack.array, QA_STACK_CAPACITY<T>);
    state = array_state::heap;
    heap.size = stack_size;
    heap.capacity = arr_size;
    heap.array = array_temp;
}

template <typename T>
QA_API void quick_array<T>::heap_resize(size_t reserve_size)
{
    T* array_temp = new T[reserve_size];
    std::memcpy(array_temp, heap.array, sizeof(T) * heap.size);
    delete[] heap.array;
    heap.array = array_temp;
    heap.capacity = reserve_size;
}

template <typename T>
QA_API T& quick_array<T>::at_impl(size_t index, size_t arr_size, T* buff)
{
    if (index >= arr_size)
        throw std::out_of_range("Index is out of bounds.");
    else
        return buff[index];
}

template <typename T>
QA_API T& quick_array<T>::back_impl(size_t last, T* buff)
{
    return buff[last - 1];
}

template <typename T>
QA_API void quick_array<T>::push_back_impl(size_t& size, T* buff, T item)
{
    buff[size] = item;
    size++;
}

template <typename T>
QA_API void quick_array<T>::quick_array_init(const std::initializer_list<T>& arr, size_t cap)
{
    if (cap * sizeof(T) > QA_STACK_CAPACITY<T>) {
        heap_init(arr.size(), cap);
        std::memcpy(heap.array, arr.begin(), sizeof(T) * arr.size());
    } else {
        stack_init(arr.size());
        std::memcpy(stack.array, arr.begin(), sizeof(T) * arr.size());
    }
}


/*
 * ===============================================================
 *
 *                   CONSTRUCTION & DESTRUCTION
 *
 * ===============================================================
 */

template <typename T>
QA_API quick_array<T>::quick_array() noexcept
{
    stack.size = 0;
}

template <typename T>
QA_API quick_array<T>::quick_array(size_t cap)
{
    is_stack() ? stack_init() : heap_init(0, cap);
}

template <typename T>
QA_API quick_array<T>::quick_array(std::initializer_list<T> arr)
{
    quick_array_init(arr, arr.size());
}

template <typename T>
QA_API quick_array<T>::quick_array(std::initializer_list<T> arr, size_t size)
{
    assert(arr.size() >= size);

    quick_array_init(arr, size);
}

template <typename T>
QA_API quick_array<T>::quick_array(const quick_array<T>& other)
{
    if (other.is_heap()) {
        heap_init(other.heap.size, other.heap.capacity);
        std::memcpy(heap.array, other.heap.array, sizeof(T) * other.heap.size);
    } else {
        stack_init(other.stack.size);
        std::memcpy(stack.array, other.stack.array, sizeof(T) * other.stack.size);
    }
}

template <typename T>
QA_API quick_array<T>::quick_array(quick_array<T>&& other) noexcept
{
    heap = other.heap;
    state = other.state;
}

template <typename T>
QA_API quick_array<T>::~quick_array() noexcept
{
    if (is_heap())
        delete[] heap.array;
}

template <typename T>
QA_API quick_array<T>& quick_array<T>::operator=(quick_array<T> other) noexcept
{
    swap(other);
    return *this;
}


/*
 * ===============================================================
 *
 *                             MODIFIERS
 *
 * ===============================================================
 */

template <typename T>
QA_API void quick_array<T>::clear() noexcept
{
    is_heap() ? heap.size = 0 : stack.size = 0;
}

template <typename T>
QA_API void quick_array<T>::insert(size_t index, T& element)
{
    this[index] = element;
}

template <typename T>
QA_API void quick_array<T>::push_back(T item)
{
    if (is_heap()) {
        if (heap.size == heap.capacity)
            heap_resize(heap.capacity * QA_GROWTH_FACTOR);
        push_back_impl(heap.size, heap.array, item);
    } else if (stack.size == stack.capacity) {
        stack_to_heap(stack.capacity * QA_GROWTH_FACTOR);
        push_back_impl(heap.size, heap.array, item);
    } else {
        /* stack_array's uint8_t size member generates rvalue temporary with push_back_impl */
        stack.array[stack.size] = item;
        stack.size++;
    }
}

template <typename T>
QA_API void quick_array<T>::pop_back() noexcept
{
    is_heap() ? heap.size -= 1 : stack.size -= 1;
}

template <typename T>
QA_API void quick_array<T>::swap(quick_array<T>& other) noexcept
{
    std::swap(heap, other.heap);
    std::swap(state, other.state);
}

template <typename T>
QA_API void swap(quick_array<T>& first, quick_array<T>& second) noexcept
{
    std::swap(first.heap, second.heap);
    std::swap(first.state, second.state);
}


/*
 * ===============================================================
 *
 *                             CAPACITY
 *
 * ===============================================================
 */

template <typename T>
QA_API constexpr bool quick_array<T>::empty() const noexcept
{
    return size() == 0;
}

template <typename T>
QA_API constexpr size_t quick_array<T>::size() const noexcept
{
    return is_heap() ? heap.size : stack.size;
}

template <typename T>
QA_API constexpr size_t quick_array<T>::max_size() noexcept
{
    return size_t(-1) / sizeof(T);
}

template <typename T>
QA_API constexpr size_t quick_array<T>::capacity() const noexcept
{
    return is_heap() ? heap.capacity : stack.capacity;
}

template <typename T>
QA_API void quick_array<T>::reserve(size_t reserve_size)
{
    if (!is_heap())
        stack_to_heap(reserve_size);
    if (reserve_size > heap.capacity)
        heap_resize(reserve_size);
}

template <typename T>
QA_API void quick_array<T>::shrink_to_fit() noexcept
{
    if (is_heap())
        heap_resize(heap.size);
}


/*
 * ===============================================================
 *
 *                             ITERATORS
 *
 * ===============================================================
 */

template <typename T>
QA_API auto quick_array<T>::begin() noexcept
{
    return data();
}

template <typename T>
QA_API auto quick_array<T>::end() noexcept
{
    return is_heap() ? &back_impl(heap.size + 1, heap.array) : &back_impl(stack.size + 1, stack.array);
}

template <typename T>
QA_API const auto quick_array<T>::begin_c() const noexcept
{
    return begin();
}

template <typename T>
QA_API const auto quick_array<T>::end_c() const noexcept
{
    return end();
}


/*
 * ===============================================================
 *
 *                        ELEMENT ACCESS
 *
 * ===============================================================
 */

template <typename T>
QA_API T& quick_array<T>::at(size_t index) noexcept
{
    return is_heap() ? at_impl(index, heap.size, heap.array) : at_impl(index, stack.size, stack.array);
}

template <typename T>
QA_API const T& quick_array<T>::at_c(size_t index) const noexcept
{
    return at(index);
}

template <typename T>
QA_API T& quick_array<T>::operator[](size_t counter)
{
    return data()[counter];
}

template <typename T>
QA_API T& quick_array<T>::front() noexcept
{
    return at(0);
}

template <typename T>
QA_API const T& quick_array<T>::front_c() const noexcept
{
    return front();
}

template <typename T>
QA_API T& quick_array<T>::back() noexcept
{
    return is_heap() ? back_impl(heap.size, heap.array) : back_impl(stack.size, stack.array);
}

template <typename T>
QA_API const T& quick_array<T>::back_c() const noexcept
{
    return back();
}

template <typename T>
QA_API T* quick_array<T>::data() noexcept
{
    return is_heap() ? heap.array : stack.array;
}

template <typename T>
QA_API const T* quick_array<T>::data_c() const noexcept
{
    return data();
}

#endif /* QUICK_ARRAY_560FDBD65BB6 */