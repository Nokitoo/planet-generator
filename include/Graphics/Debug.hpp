#pragma once

namespace Graphics {

struct Debug {
    void wireframeDisplayed(bool displayed);
    void normalsDisplayed(bool displayed);

    bool wireframeDisplayed() const;
    bool normalsDisplayed() const;
    bool isActivated() const;

private:
    void update();

private:
    bool _wireframe = false;
    bool _normals = false;

    bool _activated = false;
};

#include <Graphics/Debug.inl>

} // Namespace Graphics

