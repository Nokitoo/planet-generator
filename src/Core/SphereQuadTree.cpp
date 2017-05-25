#include <iostream> // std::cerr

#include <Graphics/API/Builder/Texture.hpp> // Graphics::API::Builder::Texture
#include <System/Vector.hpp> // System::Vector

#include <Core/SphereQuadTree.hpp> // Graphics::Core::SphereQuadTree

namespace Core {

SphereQuadTree::SphereQuadTree(float size): _size(size) {
    // Center cube
    glm::vec3 baseOffset = {
        -_size / 2.0f,
        -_size / 2.0f,
        _size / 2.0f
    };

    _leftQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(0.0f, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(0.0f, 0.0f, 1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(-1.0f, 0.0f, 0.0f), // Normal
        QuadTree::Face::LEFT,
        _levelsTable,
        0
        );
    _rightQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(_size, 0.0f, 0.0f) + baseOffset, // Position
        glm::vec3(0.0f, 0.0f, -1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(1.0f, 0.0f, 0.0f), // Normal
        QuadTree::Face::RIGHT,
        _levelsTable,
        0
        );
    _frontQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(0.0f) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Normal
        QuadTree::Face::FRONT,
        _levelsTable,
        0
        );
    _backQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(_size, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(-1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Normal
        QuadTree::Face::BACK,
        _levelsTable,
        0
        );
    _topQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(0.0f, _size, 0.0f) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Height direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Normal
        QuadTree::Face::TOP,
        _levelsTable,
        0
        );
    _bottomQuadTree = std::make_unique<Core::QuadTree>(
        _size, // Width
        glm::vec3(0.0f, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Height direction
        glm::vec3(0.0f, -1.0f, 0.0f), // Normal
        QuadTree::Face::BOTTOM,
        _levelsTable,
        0
        );

    _leftQuadTree->setNeighBors(
        _topQuadTree.get(), // Top neightbor
        _backQuadTree.get(), // Left neighbor
        _frontQuadTree.get(), // Right neighbor
        _bottomQuadTree.get() // Bottom neighbor
        );
    _rightQuadTree->setNeighBors(
        _topQuadTree.get(), // Top neightbor
        _frontQuadTree.get(), // Left neighbor
        _backQuadTree.get(), // Right neighbor
        _bottomQuadTree.get() // Bottom neighbor
        );
    _frontQuadTree->setNeighBors(
        _topQuadTree.get(), // Top neightbor
        _leftQuadTree.get(), // Left neighbor
        _rightQuadTree.get(), // Right neighbor
        _bottomQuadTree.get() // Bottom neighbor
        );
    _backQuadTree->setNeighBors(
        _topQuadTree.get(), // Top neightbor
        _rightQuadTree.get(), // Left neighbor
        _leftQuadTree.get(), // Right neighbor
        _bottomQuadTree.get() // Bottom neighbor
        );
    _topQuadTree->setNeighBors(
        _backQuadTree.get(), // Top neightbor
        _leftQuadTree.get(), // Left neighbor
        _rightQuadTree.get(), // Right neighbor
        _frontQuadTree.get() // Bottom neighbor
        );
    _bottomQuadTree->setNeighBors(
        _frontQuadTree.get(), // Top neightbor
        _leftQuadTree.get(), // Left neighbor
        _rightQuadTree.get(), // Right neighbor
        _backQuadTree.get() // Bottom neighbor
        );

    initHeightMap();
    initLevelsDistance();
    initBufferBuilder();
}

SphereQuadTree::SphereQuadTree(SphereQuadTree&& quadTree) {
    if (quadTree._leftQuadTree != nullptr) {
        _leftQuadTree = std::move(quadTree._leftQuadTree);
    }
    if (quadTree._rightQuadTree != nullptr) {
        _rightQuadTree = std::move(quadTree._rightQuadTree);
    }
    if (quadTree._frontQuadTree != nullptr) {
        _frontQuadTree = std::move(quadTree._frontQuadTree);
    }
    if (quadTree._backQuadTree != nullptr) {
        _backQuadTree = std::move(quadTree._backQuadTree);
    }
    if (quadTree._topQuadTree != nullptr) {
        _topQuadTree = std::move(quadTree._topQuadTree);
    }
    if (quadTree._bottomQuadTree != nullptr) {
        _bottomQuadTree = std::move(quadTree._bottomQuadTree);
    }

    initBufferBuilder();
    _buffer = std::move(quadTree._buffer);
    _levelsTable = quadTree._levelsTable;
}

SphereQuadTree& SphereQuadTree::operator=(SphereQuadTree&& quadTree) {
    if (quadTree._leftQuadTree != nullptr) {
        _leftQuadTree = std::move(quadTree._leftQuadTree);
    }
    if (quadTree._rightQuadTree != nullptr) {
        _rightQuadTree = std::move(quadTree._rightQuadTree);
    }
    if (quadTree._frontQuadTree != nullptr) {
        _frontQuadTree = std::move(quadTree._frontQuadTree);
    }
    if (quadTree._backQuadTree != nullptr) {
        _backQuadTree = std::move(quadTree._backQuadTree);
    }
    if (quadTree._topQuadTree != nullptr) {
        _topQuadTree = std::move(quadTree._topQuadTree);
    }
    if (quadTree._bottomQuadTree != nullptr) {
        _bottomQuadTree = std::move(quadTree._bottomQuadTree);
    }

    initBufferBuilder();
    _buffer = std::move(quadTree._buffer);
    _levelsTable = quadTree._levelsTable;

    return *this;
}

void SphereQuadTree::update(const Graphics::Camera& camera) {
    System::Vector<QuadTree::Vertex> vertices(500);
    System::Vector<uint32_t> indices(500);

    if (isFacingCamera(_leftQuadTree.get(), camera)) {
        _leftQuadTree->update(vertices, indices, camera);
    }
    if (isFacingCamera(_rightQuadTree.get(), camera)) {
        _rightQuadTree->update(vertices, indices, camera);
    }
    if (isFacingCamera(_frontQuadTree.get(), camera)) {
        _frontQuadTree->update(vertices, indices, camera);
    }
    if (isFacingCamera(_backQuadTree.get(), camera)) {
        _backQuadTree->update(vertices, indices, camera);
    }
    if (isFacingCamera(_topQuadTree.get(), camera)) {
        _topQuadTree->update(vertices, indices, camera);
    }
    if (isFacingCamera(_bottomQuadTree.get(), camera)) {
        _bottomQuadTree->update(vertices, indices, camera);
    }

    _bufferBuilder.setVertices(
        (char*)vertices.data(),
        static_cast<uint32_t>(vertices.size()) * sizeof(QuadTree::Vertex)
        );
    _bufferBuilder.setIndices(
        (char*)indices.data(),
        static_cast<uint32_t>(indices.size()) * sizeof(uint32_t)
        );

    Graphics::API::Buffer buffer;
    if (!_bufferBuilder.build(buffer)) {
        // TODO: replace this with logger
        std::cerr << "SphereQuadTree::update: failed to create VAO" << std::endl;
        // TODO return bool
        return;
    }

    _buffer = std::move(buffer);
}

const Graphics::API::Buffer& SphereQuadTree::getBuffer() const {
    return _buffer;
}

float SphereQuadTree::getSize() const {
    return (_size);
}

const Graphics::API::Texture& SphereQuadTree::getHeightMap() const {
    return _heightMap;
}

bool SphereQuadTree::initHeightMap() {
    Graphics::API::Builder::Texture textureBuilder;

    textureBuilder.setType(GL_TEXTURE_CUBE_MAP);
    textureBuilder.setFormat(GL_RGB);
    textureBuilder.setInternalFormat(GL_RGB);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X)->setFileName("resources/images/brush.jpg");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X)->setFileName("resources/images/brush.jpg");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y)->setFileName("resources/images/brush.jpg");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)->setFileName("resources/images/brush.jpg");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z)->setFileName("resources/images/brush.jpg");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)->setFileName("resources/images/brush.jpg");
    textureBuilder.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureBuilder.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (!textureBuilder.build(_heightMap)) {
        // TODO: replace this with logger
        std::cerr << "SphereQuadTree::initHeightMap: failed to create height map texture" << std::endl;
        return false;
    }
    return true;
}

void SphereQuadTree::initLevelsDistance() {
    uint32_t maxLevels = 4;
    float distanceSteps = _size / 2.0f;
    float distance = distanceSteps * maxLevels;

    // We don't want the first 3 levels to be displayed
    // It don't look like a sphere
    _levelsTable.push_back(9999.0f);
    _levelsTable.push_back(9999.0f);
    _levelsTable.push_back(9999.0f);

    for (uint32_t i = 0; i < maxLevels; ++i) {
        _levelsTable.push_back(distance);
        distance -= distanceSteps;
    }
}

void SphereQuadTree::initBufferBuilder() {
    // Position attribute
    _bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, pos)
    });
    // QuadTree level attribute
    _bufferBuilder.addAttribute({
        1,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, quadTreeLevel)
    });
}

bool SphereQuadTree::isFacingCamera(const QuadTree* quadTree, const Graphics::Camera& camera) {
    glm::vec3 cameraDir = glm::normalize(quadTree->_center - camera.getPos());
    return glm::dot(-cameraDir, quadTree->_normal) > -0.75f;
}

} // Namespace Core

