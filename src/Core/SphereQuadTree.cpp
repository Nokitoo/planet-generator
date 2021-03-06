#include <iostream> // std::cerr

#include <Graphics/API/Builder/Buffer.hpp> // Graphics::API::Builder::Buffer
#include <Graphics/API/Builder/Texture.hpp> // Graphics::API::Builder::Texture
#include <Graphics/Renderer.hpp> // Graphics::Renderer
#include <System/Vector.hpp> // System::Vector

#include <Core/SphereQuadTree.hpp> // Graphics::Core::SphereQuadTree

namespace Core {

SphereQuadTree::SphereQuadTree(float size, float maxHeight): _size(size), _maxHeight(maxHeight) {}

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

    _buffer = std::move(quadTree._buffer);
    _levelsTable = quadTree._levelsTable;
    _maxHeight = quadTree._maxHeight;
    _heightMap = std::move(quadTree._heightMap);
    _normalMap = std::move(quadTree._normalMap);
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

    _buffer = std::move(quadTree._buffer);
    _levelsTable = quadTree._levelsTable;
    _maxHeight = quadTree._maxHeight;
    _heightMap = std::move(quadTree._heightMap);
    _normalMap = std::move(quadTree._normalMap);

    return *this;
}

std::unique_ptr<SphereQuadTree> SphereQuadTree::create(const Graphics::Renderer* renderer, float size, float maxHeight) {
    // Don't use std::make_unique because the constructor is private
    std::unique_ptr<SphereQuadTree> sphereQuadTree(new SphereQuadTree(size, maxHeight));

    if (!sphereQuadTree->init(renderer)) {
        return nullptr;
    }

    return sphereQuadTree;
}

void SphereQuadTree::update(Graphics::Camera& camera) {
    // Add the quadtrees vertices to the SphereQuadTree buffer
    {
        uint32_t chunkSize = 500;
        uint32_t verticesNb = _buffer.getVerticesNb() ? _buffer.getVerticesNb() : chunkSize;
        uint32_t indicesNb = _buffer.getIndicesNb() ? _buffer.getIndicesNb() : chunkSize;

        // Create indices and vertices buffers with chunks of 500
        // (resize is perform only every 500 vertices/indices added in the vector)
        // We also init the vector with the previous frame size to reduce the resizes
        System::Vector<QuadTree::Vertex> vertices(chunkSize, verticesNb + chunkSize);
        System::Vector<uint32_t> indices(chunkSize, indicesNb + chunkSize);

        _leftQuadTree->update(camera);
        _leftQuadTree->addChildrenVertices(vertices, indices);

        _rightQuadTree->update(camera);
        _rightQuadTree->addChildrenVertices(vertices, indices);

        _frontQuadTree->update(camera);
        _frontQuadTree->addChildrenVertices(vertices, indices);

        _backQuadTree->update(camera);
        _backQuadTree->addChildrenVertices(vertices, indices);

        _topQuadTree->update(camera);
        _topQuadTree->addChildrenVertices(vertices, indices);

        _bottomQuadTree->update(camera);
        _bottomQuadTree->addChildrenVertices(vertices, indices);

        _buffer.updateVertices(
            (char*)vertices.data(),
            static_cast<uint32_t>(vertices.size()) * sizeof(QuadTree::Vertex),
            static_cast<uint32_t>(vertices.size()),
            GL_DYNAMIC_DRAW
            );
        _buffer.updateIndices(
            (char*)indices.data(),
            static_cast<uint32_t>(indices.size()) * sizeof(uint32_t),
            static_cast<uint32_t>(indices.size()),
            GL_DYNAMIC_DRAW
            );
    }

    // Add the quadtrees debug aabb boxes vertices to the SphereQuadTree buffer
    {
        uint32_t chunkSize = 500;
        uint32_t verticesNb = _debugBuffer.getVerticesNb() ? _debugBuffer.getVerticesNb() : chunkSize;
        uint32_t indicesNb = _debugBuffer.getIndicesNb() ? _debugBuffer.getIndicesNb() : chunkSize;

        // Create indices and vertices buffers with chunks of 500
        // (resize is perform only every 500 vertices/indices added in the vector)
        // We also init the vector with the previous frame size to reduce the resizes
        System::Vector<glm::vec3> vertices(chunkSize, verticesNb + chunkSize);
        System::Vector<uint32_t> indices(chunkSize, indicesNb + chunkSize);

        _leftQuadTree->addDebugVertices(vertices, indices);
        _rightQuadTree->addDebugVertices(vertices, indices);
        _frontQuadTree->addDebugVertices(vertices, indices);
        _backQuadTree->addDebugVertices(vertices, indices);
        _topQuadTree->addDebugVertices(vertices, indices);
        _bottomQuadTree->addDebugVertices(vertices, indices);

        _debugBuffer.updateVertices(
            (char*)vertices.data(),
            static_cast<uint32_t>(vertices.size()) * sizeof(glm::vec3),
            static_cast<uint32_t>(vertices.size()),
            GL_DYNAMIC_DRAW
            );
        _debugBuffer.updateIndices(
            (char*)indices.data(),
            static_cast<uint32_t>(indices.size()) * sizeof(uint32_t),
            static_cast<uint32_t>(indices.size()),
            GL_DYNAMIC_DRAW
            );
    }
}

float SphereQuadTree::getSize() const {
    return (_size);
}

float SphereQuadTree::getMaxHeight() const {
    return (_maxHeight);
}

const Graphics::API::Buffer& SphereQuadTree::getBuffer() const {
    return _buffer;
}

const Graphics::API::Buffer& SphereQuadTree::getDebugBuffer() const {
    return _debugBuffer;
}

const QuadTree::LevelsTable& SphereQuadTree::getLevelsTable() const {
    return _levelsTable;
}

const Graphics::API::Texture& SphereQuadTree::getHeightMap() const {
    return _heightMap;
}

const Graphics::API::Texture& SphereQuadTree::getNormalMap() const {
    return _normalMap;
}

void SphereQuadTree::setMaxHeight(float maxHeight) {
    _maxHeight = maxHeight;

    _leftQuadTree->updateShapeAABB();
    _rightQuadTree->updateShapeAABB();
    _frontQuadTree->updateShapeAABB();
    _backQuadTree->updateShapeAABB();
    _topQuadTree->updateShapeAABB();
    _bottomQuadTree->updateShapeAABB();
}

void SphereQuadTree::setSize(float size) {
    _size = size;

    _leftQuadTree = nullptr;
    _rightQuadTree = nullptr;
    _frontQuadTree = nullptr;
    _backQuadTree = nullptr;
    _topQuadTree = nullptr;

    initLevelsDistance();
    initChildren();
}

bool SphereQuadTree::init(const Graphics::Renderer* renderer) {
    initLevelsDistance();
    initChildren();

    return initHeightMap() && initNormalMap(renderer) && initBuffer() && initDebugBuffer();
}

void SphereQuadTree::initChildren() {
    // Center sphere
    glm::vec3 baseOffset = {
        -_size / 2.0f,
        -_size / 2.0f,
        _size / 2.0f
    };

    _leftQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::LEFT, // Face
        0, // Level
        _size, // Size
        glm::vec3(0.0f, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(0.0f, 0.0f, 1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(-1.0f, 0.0f, 0.0f) // Normal
    );
    _rightQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::RIGHT, // Face
        0, // Level
        _size, // Size
        glm::vec3(_size, 0.0f, 0.0f) + baseOffset, // Position
        glm::vec3(0.0f, 0.0f, -1.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(1.0f, 0.0f, 0.0f) // Normal
    );
    _frontQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::FRONT, // Face
        0, // Level
        _size, // Size
        glm::vec3(0.0f) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, 1.0f) // Normal
        );
    _backQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::BACK, // Face
        0, // Level
        _size, // Size
        glm::vec3(_size, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(-1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 1.0f, 0.0f), // Height direction
        glm::vec3(0.0f, 0.0f, -1.0f) // Normal
    );
    _topQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::TOP, // Face
        0, // Level
        _size, // Size
        glm::vec3(0.0f, _size, 0.0f) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, -1.0f), // Height direction
        glm::vec3(0.0f, 1.0f, 0.0f) // Normal
    );
    _bottomQuadTree = std::make_unique<Core::QuadTree>(
        *this, // Planet
        QuadTree::Face::BOTTOM, // Face
        0, // Level
        _size, // Size
        glm::vec3(0.0f, 0.0f, -_size) + baseOffset, // Position
        glm::vec3(1.0f, 0.0f, 0.0f), // Width direction
        glm::vec3(0.0f, 0.0f, 1.0f), // Height direction
        glm::vec3(0.0f, -1.0f, 0.0f) // Normal
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
}

bool SphereQuadTree::initHeightMap() {
    Graphics::API::Builder::Texture textureBuilder;

    textureBuilder.setType(GL_TEXTURE_CUBE_MAP);
    textureBuilder.setFormat(GL_RGBA);
    textureBuilder.setInternalFormat(GL_RGBA32F);
    textureBuilder.setDataType(GL_FLOAT);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X)->setFileName("resources/images/brush3.png");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X)->setFileName("resources/images/brush3.png");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y)->setFileName("resources/images/brush3.png");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y)->setFileName("resources/images/brush3.png");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z)->setFileName("resources/images/brush3.png");
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)->setFileName("resources/images/brush3.png");
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

bool SphereQuadTree::initNormalMap(const Graphics::Renderer* renderer) {
    Graphics::API::Builder::Texture textureBuilder;

    textureBuilder.setType(GL_TEXTURE_CUBE_MAP);
    textureBuilder.setFormat(GL_RGBA);
    textureBuilder.setInternalFormat(GL_RGBA32F);
    textureBuilder.setDataType(GL_FLOAT);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    textureBuilder.addImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    textureBuilder.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    textureBuilder.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    textureBuilder.setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textureBuilder.setWidth(_heightMap.getWidth());
    textureBuilder.setHeight(_heightMap.getHeight());

    if (!textureBuilder.build(_normalMap)) {
        // TODO: replace this with logger
        std::cerr << "SphereQuadTree::initNormalMap: failed to create normal map texture" << std::endl;
        return false;
    }

    renderer->createNormalMapFromHeightMap(_heightMap, _normalMap, _maxHeight);

    return true;
}

void SphereQuadTree::initLevelsDistance() {
    uint32_t maxLevels = 5;
    float distance = _size / 0.2f;

    _levelsTable.clear();

    // We don't want the first 3 levels to be displayed
    // It don't look like a sphere
    _levelsTable.push_back(distance);
    _levelsTable.push_back(distance);
    _levelsTable.push_back(distance);

    for (uint32_t i = 0; i < maxLevels; ++i) {
        _levelsTable.push_back(distance);
        distance /= 2.0f;
    }
}

bool SphereQuadTree::initBuffer() {
    Graphics::API::Builder::Buffer bufferBuilder;

    // Cube position attribute
    bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, cubePos)
    });
    // Sphere position attribute
    bufferBuilder.addAttribute({
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, spherePos)
    });
    // Width direction attribute
    bufferBuilder.addAttribute({
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, widthDir)
    });
    // Height direction attribute
    bufferBuilder.addAttribute({
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, heightDir)
    });
    // QuadTree level attribute
    bufferBuilder.addAttribute({
        4,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(QuadTree::Vertex),
        offsetof(QuadTree::Vertex, quadTreeLevel)
    });

    bufferBuilder.setVerticesUsage(GL_DYNAMIC_DRAW);
    bufferBuilder.setIndicesUsage(GL_DYNAMIC_DRAW);

    if (!bufferBuilder.build(_buffer)) {
        // TODO: replace this with logger
        std::cerr << "SphereQuadTree::initBuffer: failed to create VAO" << std::endl;
        return false;
    }

    return true;
}

bool SphereQuadTree::initDebugBuffer() {
    Graphics::API::Builder::Buffer bufferBuilder;

    // Position attribute
    bufferBuilder.addAttribute({
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        0
    });

    bufferBuilder.setVerticesUsage(GL_DYNAMIC_DRAW);
    bufferBuilder.setIndicesUsage(GL_DYNAMIC_DRAW);

    if (!bufferBuilder.build(_debugBuffer)) {
        // TODO: replace this with logger
        std::cerr << "SphereQuadTree::initDebugBuffer: failed to create VAO" << std::endl;
        return false;
    }

    return true;
}

} // Namespace Core

