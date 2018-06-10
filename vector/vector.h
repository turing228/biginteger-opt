//
// Created by jesus on 09.06.18.
//

#ifndef UNTITLED_VECTOR_H
#define UNTITLED_VECTOR_H
#define small_size 4

#include <iostream>
#include <cassert>
#include <cstring>
#include <memory>

typedef unsigned int T;

template<typename U>
void copy_all(U *destination, U const *source, size_t size,
              typename std::enable_if<!std::is_trivially_copyable<U>::value>::type * = nullptr) {
    for (size_t i = 0; i != size; ++i)
        new(destination + i)U(source[i]);
}

template<typename U>
void copy_all(U *destination, U const *source, size_t size,
              typename std::enable_if<std::is_trivially_copyable<U>::value>::type * = nullptr) {
    if (size != 0)
        memcpy(destination, source, sizeof(U) * size);
}


struct vector {
    //typedef T *iterator;
    //typedef T const *const_iterator;
    //typedef std::reverse_iterator<iterator> reverse_iterator;
    //typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private:
    //T *_data;
    size_t _size;
    //size_t _capacity;
    //size_t increase_capacity() const;

    void increase_capacity();

    bool is_big;

    struct big_data {
        size_t capacity;

        std::shared_ptr<T[]> pointer;

        big_data() {}

        big_data(T *p, size_t n) : pointer(p), capacity(n) {}

        big_data(const big_data &other) : pointer(other.pointer), capacity(other.capacity) {}
    };

    union gen_data {
        big_data big;
        T small[small_size]{};

        gen_data() {}

        ~gen_data() {}
    } data;

    T *_data;

public:
    //typedef unsigned int T;

    vector();

    //explicit vector(size_t);

    vector(size_t, T value = T());

    vector(vector const &other);

    ~vector();

    void push_back(T const &);

    void pop_back();

    T back();

    T const back() const;

    T &get(size_t);

    bool empty() const;

    T &operator[](size_t);

    T const &operator[](size_t) const;

    size_t size() const;

    size_t capacity() const;

    //T *data();

    //T const *data() const;

    void reserve(size_t);

    //void shrink_to_fit();

    //void clear();

    void resize(size_t, T value = T());

    //void assign(size_t, T value = T());

    void swap(vector &) noexcept;

    void swap_big_small(gen_data &big, gen_data &small) noexcept;

    friend bool operator==(const vector &, const vector &);

    vector &operator=(const vector &);

    //void swap(vector &a, vector &b);

    //void copy_all(T*, T *, size_t);
};


#endif //UNTITLED_VECTOR_H
