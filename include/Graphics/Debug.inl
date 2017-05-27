inline void Debug::wireframeDisplayed(bool displayed) {
    _wireframe = displayed;
    update();
}

inline void Debug::normalsDisplayed(bool displayed) {
    _normals = displayed;
    update();
}

inline bool Debug::wireframeDisplayed() const {
    return _wireframe;
}

inline bool Debug::normalsDisplayed() const {
    return _normals;
}

inline bool Debug::isActivated() const {
    return _activated;
}

inline void Debug::update() {
    _activated = _wireframe || _normals;
}
