template<typename T>
inline Vector<T>::Vector(uint32_t chunkSize, uint32_t baseSize): _chunkSize(chunkSize) {
    if (baseSize) {
        _vector.resize(baseSize);
    }
}

template<typename T>
inline void Vector<T>::push_back(T elem) {
    uint32_t vectorSize = static_cast<uint32_t>(_vector.size());

    if (!_elemsNb || _elemsNb == vectorSize) {
        _vector.resize(vectorSize + _chunkSize);
    }

    _vector[_elemsNb] = elem;
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
