inline void Texture::Image::setType(GLenum type) {
    _type = type;
}

inline void  Texture::Image::setFileName(const std::string& fileName) {
    _fileName = fileName;
}

inline void Texture::setType(GLenum type) {
    _type = type;
}

inline void Texture::setWidth(GLsizei width) {
    _width = width;
}

inline void Texture::setHeight(GLsizei height) {
    _height = height;
}

inline void Texture::setInternalFormat(GLint internalFormat) {
    _internalFormat = internalFormat;
}

inline void Texture::setFormat(GLint format) {
    _format = format;
}

inline void Texture::setDataType(GLenum dataType) {
    _dataType = dataType;
}

inline void Texture::setParameter(GLenum paramName, GLint paramValue) {
    _parameters[paramName] = paramValue;
}

template<typename... Args>
inline Texture::Image* Texture::addImage(Args... args) {
    _images.push_back(Texture::Image(args...));
    _images.back()._texture = this;
    return &_images.back();
}
