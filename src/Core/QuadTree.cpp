#include <Core/QuadTree.hpp> // Graphics::Core::QuadTree

namespace Core {

QuadTree::QuadTree(float size,
                    const glm::vec3& pos,
                    const glm::vec3& widthDir,
                    const glm::vec3& heightDir,
                    const glm::vec3& normal): _size(size), _pos(pos), _widthDir(widthDir), _heightDir(heightDir), _normal(normal) {
    glm::vec3 topLeft = pos + (heightDir * size);
    glm::vec3 topRight = pos + (widthDir * size) + (heightDir * size);
    glm::vec3 bottomLeft = pos;
    glm::vec3 bottomRight = pos + (widthDir * size);

    _corners[0] = {
        topLeft, glm::vec3(1.0f, 0.0f, 0.0f), _normal
    };
    _corners[1] = {
        topRight, glm::vec3(0.0f, 1.0f, 0.0f), _normal
    };
    _corners[2] = {
        bottomLeft, glm::vec3(0.0f, 0.0f, 1.0f), normal
    };
    _corners[3] = {
        bottomRight, glm::vec3(1.0f, 1.0f, 0.0f), _normal
    };
}

void QuadTree::update(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t level) {
    if (!level) {
        if (_split) {
            merge();
        }
        return;
    }

    if (!_split) {
        split();
    }

    uint32_t verticesNb = static_cast<uint32_t>(vertices.size());

    vertices.push_back(_corners[0]);
    vertices.push_back(_corners[1]);
    vertices.push_back(_corners[2]);
    vertices.push_back(_corners[3]);

    indices.push_back(verticesNb);
    indices.push_back(verticesNb + 2);
    indices.push_back(verticesNb + 3);
    indices.push_back(verticesNb + 3);
    indices.push_back(verticesNb + 1);
    indices.push_back(verticesNb);

    _children.topLeft->update(vertices, indices, level - 1);
    _children.topRight->update(vertices, indices, level - 1);
    _children.bottomLeft->update(vertices, indices, level - 1);
    _children.bottomRight->update(vertices, indices, level - 1);
}

void QuadTree::split() {
    float childrenSize = _size / 2;
    _children.topLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );
    _children.topRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize) + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );
    _children.bottomLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos,
        _widthDir,
        _heightDir,
        _normal
        );
    _children.bottomRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal
        );

    _split = true;
}

void QuadTree::merge() {
    _children.topLeft = nullptr;
    _children.topRight = nullptr;
    _children.bottomLeft = nullptr;
    _children.bottomRight = nullptr;

    _split = false;
}

} // Core

