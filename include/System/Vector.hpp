#pragma once

#include <vector> // std::vector

namespace System {

/*
 *
 * Abstraction of std::vector to resize the vector every chunkSize
 *
*/
template<typename T>
class Vector {
public:
    Vector(uint32_t chunkSize, uint32_t baseSize = 0);
    ~Vector() = default;

    Vector(const Vector& vector) = delete;
    Vector(Vector&& vector) = delete;

    Vector& operator=(const Vector& vector) = delete;
    Vector& operator=(Vector&& vector) = delete;

    void push_back(T elem);
    uint32_t size() const;
    const T* data() const;

private:
    uint32_t _chunkSize;

    std::vector<T> _vector;

    uint32_t _elemsNb = 0;
    uint32_t _vectorCapacity = 0;
};

#include <System/Vector.inl>

} // Namespace System

