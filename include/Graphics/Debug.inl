inline void Debug::wireframeDisplayed(bool displayed) {
    _wireframe = displayed;
    update();
}

inline void Debug::verticesNormalsDisplayed(bool displayed) {
    _verticesNormals = displayed;
    update();
}

inline void Debug::facesNormalsDisplayed(bool displayed) {
    _facesNormals = displayed;
    update();
}

inline bool Debug::wireframeDisplayed() const {
    return _wireframe;
}

inline bool Debug::verticesNormalsDisplayed() const {
    return _verticesNormals;
}

inline bool Debug::facesNormalsDisplayed() const {
    return _facesNormals;
}

inline bool Debug::isActivated() const {
    return _activated;
}

inline void Debug::update() {
    _activated = _wireframe || _verticesNormals || _facesNormals;
}
