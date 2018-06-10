//
// Created by jesus on 10.06.18.
//
#include "vector.h"

vector::vector() {
    _data = data.small;
    _size = 0;
    is_big = false;
}


/*vector::vector(size_t n) {
    reserve(n);
    std::memset(_data, 0, n * sizeof(T));
    _size = n;
}*/

vector::vector(size_t n, T value) : vector() {
    reserve(n);
    std::memset(_data, 0, n * sizeof(T));
    _size = n;
    for (size_t i = 0; i < n; ++i) {
        _data[i] = value;
    }
}


vector::vector(vector const &other) {
    if (other.is_big) {
        is_big = true;
        new(&data.big) big_data(other.data.big);
        _data = data.big.pointer.get();
    } else {
        is_big = false;
        std::memcpy(data.small, other.data.small, small_size * sizeof(T));
        _data = data.small;
    }
    _size = other._size;
    /*vector();
    new_vector(other.size());
    copy_all(_data, other._data, other._size);
    _size = other._size;*/
}


vector::~vector() {
    if (is_big) {
        data.big.~big_data();
    }
}

/*
void vector::new_vector(size_t new_capacity) {
    assert(new_capacity >= _size);
    vector temp;
    if (new_capacity != 0) {
        temp._data = (T *) operator new(sizeof(T) * new_capacity);
        temp.capacity() = new_capacity;
        copy_all(temp._data, _data, _size);
        temp._size = _size;
    }
    swap(temp);
}*/

T *clone(T *data, size_t n, size_t size) {
    auto *copy = new T[n];
    std::memcpy(copy, data, size * sizeof(T));
    return copy;
}

void vector::increase_capacity() {
    if (is_big && !data.big.pointer.unique()) {
        data.big.pointer.reset(clone(_data, data.big.capacity * 2, _size));
        data.big.capacity *= 2;
        _data = data.big.pointer.get();
    }
}


void vector::push_back(const T &value) {
    if (_size >= capacity()) {
        reserve(capacity() * 2);
        //new(_data + _size)T(value);
        //++_size;
    } else {
        increase_capacity();
        /*vector temp;
        temp.new_vector(increase_capacity());
        copy_all(temp._data, _data, _size);
        temp._size = _size;
        temp.push_back(value);
        swap(temp);*/
    }
    _data[_size++] = value;
}


void vector::pop_back() {
    _size--;
}


T vector::back() {
    return _data[_size - 1];
}


T const vector::back() const {
    return _data[_size - 1];
}


T &vector::get(size_t i) {
    return _data[i];
}


bool vector::empty() const {
    return _size == 0;
}


T &vector::operator[](size_t n) {
    increase_capacity();
    return _data[n];
}


T const &vector::operator[](size_t n) const {
    return _data[n];
}


size_t vector::size() const {
    return _size;
}


size_t vector::capacity() const {
    return is_big ? data.big.capacity : small_size;
}


void vector::reserve(size_t n) {
    /*if (n < get_capacity()) {
        return;
    } else {
        new_vector(n);
    }*/
    if (n > capacity() && n > small_size) {
        n = std::max(n, capacity() * 2);
        if (is_big) {
            data.big.pointer.reset(clone(_data, n, _size));
            data.big.capacity = n;
        } else {
            new(&data.big)
                    big_data(clone(_data, n, _size), n);
            is_big = true;
        }
        _data = data.big.pointer.get();
    }
}

void vector::resize(size_t n, T value) {
    vector temp(n);
    if (_size > n) {
        copy_all(temp._data, _data, n);
    } else {
        copy_all(temp._data, _data, _size);
        for (size_t i = _size; i != n; ++i) {
            temp._data[i] = value;
        }
    }
    swap(temp);
}

void vector::swap_big_small(gen_data &big, gen_data &small) noexcept {
    T temp[small_size];
    std::memcpy(temp, small.small, small_size * sizeof(T));
    new(&small.big) big_data(big.big);
    big.big.~big_data();
    std::memcpy(big.small, temp, small_size * sizeof(T));
}

int cond_numb(bool a, bool b) {
    int x = 0;
    int y = 0;
    if (a) x = 10;
    if (b) y = 1;
    return x + y;
}

void vector::swap(vector &other) noexcept {
    switch (cond_numb(is_big, other.is_big)) {
        case 00 :
            std::swap(data.small, other.data.small);
            break;
        case 01 :
            swap_big_small(other.data, data);
            _data = data.big.pointer.get();
            other._data = other.data.small;
            break;
        case 10 :
            swap_big_small(data, other.data);
            _data = data.small;
            other._data = other.data.big.pointer.get();
            break;
        case 11 :
            std::swap(data.big, other.data.big);
            _data = data.big.pointer.get();
            other._data = other.data.big.pointer.get();
            break;
    }
    std::swap(is_big, other.is_big);
    std::swap(_size, other._size);
}

bool operator==(const vector &a, const vector &b) {
    if (a._size != b._size) {
        return false;
    }
    return (memcmp(a._data, b._data, a._size * sizeof(T)) == 0);
}

vector &vector::operator=(vector const &other) {
    vector temp(other);
    swap(temp);
    return *this;
}