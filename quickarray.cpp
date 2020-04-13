#include "quickarray.h"
#include <iostream>

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
    if (is_heap())
        delete[] hvec.array;
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
void quick_array<T>::heap_enlarge(size_t reserve_size)
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
        throw std::runtime_error("Index is out of bounds.");
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
quick_array<T>::quick_array(std::initializer_list<T> arr)
{
    quick_array_init(arr, arr.size());
}

template <typename T>
quick_array<T>::quick_array(std::initializer_list<T> arr, size_t size)
{
    quick_array_init(arr, size);
}

template <typename T>
quick_array<T>::quick_array(quick_array& other)
{
    if (other.is_heap()) {
        state = vec_state::heap;
        this->hvec.size = other.hvec.size;
        this->hvec.capacity = other.hvec.capacity;
        std::memcpy(this->hvec.array = other.hvec.array, sizeof(T) * other.hvec.size);
    } else {
        state = vec_state::stack;
        this->svec.capacity = other.capacity;
        this->svec.size = other.size;
        std::memcpy(this->svec.array, other.svec.array, sizeof(T) * other.svec.size);
    }
}

template <typename T>
quick_array<T>::quick_array(quick_array&& other)
{
    this->hvec = &other->hvec;
    &(other->hvec) = nullptr;
    state = other.state;
}

template <typename T>
quick_array<T>::~quick_array()
{
    if (is_heap())
        delete[] hvec.array;
}

template <typename T>
quick_array<T>& quick_array<T>::operator=(quick_array other)
{
    std::swap(this, &other);
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
            heap_enlarge(hvec.capacity * 2);
        push_back_impl(hvec.size, hvec.array, item);
    } else {
        if (svec.size == svec.capacity) { //stack arr full
            stack_to_heap(stack_bytes<T> * 2);
            push_back_impl(hvec.size, hvec.array, item);
        } else {
            svec.array[svec.size] = item;
            svec.size++;
        }
    }
}

template <typename T>
void quick_array<T>::pop_back()
{
    size() -= 1;
}

template <typename T>
void quick_array<T>::swap(quick_array& other) noexcept
{
    std::swap(*this, other);
}

template <typename T>
constexpr bool quick_array<T>::empty() const noexcept
{
    return size() == 0;
}

template <typename T>
constexpr size_t quick_array<T>::size() const noexcept
{
    return is_heap() ? hvec.size : svec.size;
}

template <typename T>
constexpr size_t quick_array<T>::max_size() noexcept
{
    return -1;
}

template <typename T>
constexpr size_t quick_array<T>::capacity() const noexcept
{
    return is_heap() ? hvec.capacity : svec.capacity;
}

template <typename T>
void quick_array<T>::reserve(size_t reserve_size) //cant reserve stack, no need to check state
{
    if (!is_heap())
        stack_to_heap(reserve_size);
    if (reserve_size > hvec.capacity)
        heap_enlarge(reserve_size);
}

template <typename T>
void quick_array<T>::shrink_to_fit() noexcept
{
    if (is_heap())
        heap_enlarge(hvec.size);
}

template <typename T>
auto quick_array<T>::begin() noexcept
{
    return data();
}

template <typename T>
auto quick_array<T>::end() noexcept
{
    return is_heap() ? &back_impl(hvec.size + 1, hvec.array) : &back_impl(svec.size + 1, hvec.array);
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