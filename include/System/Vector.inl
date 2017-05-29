template<typename T>
inline Vector<T>::Vector(uint32_t chunkSize, uint32_t baseSize): _chunkSize(chunkSize) {
    if (baseSize) {
        _vector.reserve(baseSize);
    }
    _vectorCapacity = baseSize;
}

template<typename T>
inline void Vector<T>::push_back(T elem) {
    if (!_elemsNb || _elemsNb == _vectorCapacity) {
        _vectorCapacity += _chunkSize;
        _vector.resize(_vectorCapacity);
    }

    _vector[_elemsNb] = std::move(elem);
    ++_elemsNb;
}

template<typename T>
inline uint32_t Vector<T>::size() const {
    return _elemsNb;
}

template<typename T>
inline const T* Vector<T>::data() const {
    return _vector.data();
}
