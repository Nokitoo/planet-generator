#pragma once

namespace Graphics {

struct Debug {
    void wireframeDisplayed(bool displayed);
    void verticesNormalsDisplayed(bool displayed);
    void facesNormalsDisplayed(bool displayed);

    bool wireframeDisplayed() const;
    bool verticesNormalsDisplayed() const;
    bool facesNormalsDisplayed() const;
    bool isActivated() const;

private:
    void update();

private:
    bool _wireframe = false;
    bool _verticesNormals = false;
    bool _facesNormals = false;

    bool _activated = false;
};

#include <Graphics/Debug.inl>

} // Namespace Graphics

