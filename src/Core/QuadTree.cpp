#include <unordered_map>
#include <Core/QuadTree.hpp> // Graphics::Core::QuadTree

namespace Core {

QuadTree::QuadTree(float size,
                    const glm::vec3& pos,
                    const glm::vec3& widthDir,
                    const glm::vec3& heightDir,
                    const glm::vec3& normal,
                    Face face,
                    const LevelsTable& levelsTable,
                    uint32_t level,
                    QuadTree* parent):
    _size(size), _pos(pos), _widthDir(widthDir), _heightDir(heightDir), _normal(normal), _face(face), _levelsTable(levelsTable), _level(level), _parent(parent) {
    glm::vec3 topLeft = pos + (heightDir * size);
    glm::vec3 topRight = pos + (widthDir * size) + (heightDir * size);
    glm::vec3 bottomLeft = pos;
    glm::vec3 bottomRight = pos + (widthDir * size);

    _corners[0] = {
        topLeft, static_cast<float>(_level)
    };
    _corners[1] = {
        topRight, _corners[0].quadTreeLevel
    };
    _corners[2] = {
        bottomLeft, _corners[0].quadTreeLevel
    };
    _corners[3] = {
        bottomRight, _corners[0].quadTreeLevel
    };

    _center = bottomLeft + (widthDir * size / 2.0f) + (heightDir * size / 2.0f);
}

void QuadTree::update(System::Vector<Vertex>& vertices, System::Vector<uint32_t>& indices, const Graphics::Camera& camera) {
    if (!isInsideFrustrum(camera)) {
        if (_split) {
            merge();
        }
        return;
    }

    if (needSplit(camera)) {
        split();
    }
    else if (needMerge(camera)) {
        merge();
    }

    if (_split) {
        _children.topLeft->update(vertices, indices, camera);
        _children.topRight->update(vertices, indices, camera);
        _children.bottomLeft->update(vertices, indices, camera);
        _children.bottomRight->update(vertices, indices, camera);
        addChildrenVertices(vertices, indices);
    }

}

void QuadTree::updateNeighBors() {
    // Remove all neighbors from current quadtree neighbors children
    {
        if (!_split) {
            if (_children.topLeft->_neighbors.top != nullptr) {
                _children.topLeft->_neighbors.top->setNeighbor(_face, NeighborOrientation::BOTTOM, nullptr);
            }
            if (_children.topLeft->_neighbors.left != nullptr) {
                _children.topLeft->_neighbors.left->setNeighbor(_face, NeighborOrientation::RIGHT, nullptr);
            }

            if (_children.topRight->_neighbors.top != nullptr) {
                _children.topRight->_neighbors.top->setNeighbor(_face, NeighborOrientation::BOTTOM, nullptr);
            }
            if (_children.topRight->_neighbors.right != nullptr) {
                _children.topRight->_neighbors.right->setNeighbor(_face, NeighborOrientation::LEFT, nullptr);
            }

            if (_children.bottomLeft->_neighbors.left != nullptr) {
                _children.bottomLeft->_neighbors.left->setNeighbor(_face, NeighborOrientation::RIGHT, nullptr);
            }
            if (_children.bottomLeft->_neighbors.bottom != nullptr) {
                _children.bottomLeft->_neighbors.bottom->setNeighbor(_face, NeighborOrientation::TOP, nullptr);
            }

            if (_children.bottomRight->_neighbors.right != nullptr) {
                _children.bottomRight->_neighbors.right->setNeighbor(_face, NeighborOrientation::LEFT, nullptr);
            }
            if (_children.bottomRight->_neighbors.bottom != nullptr) {
                _children.bottomRight->_neighbors.bottom->setNeighbor(_face, NeighborOrientation::TOP, nullptr);
            }

            return;
        }
    }

    // Update neighbors of current quadtree children and neighbors children
    {
        // Left neighbor
        if (_neighbors.left != nullptr && _neighbors.left->_split) {
            QuadTree* neighborChildTopRight = _neighbors.left->getChild(_face, ChildOrientation::TOP_RIGHT);
            QuadTree* neighborChildBottomRight = _neighbors.left->getChild(_face, ChildOrientation::BOTTOM_RIGHT);

            _children.topLeft->_neighbors.left = neighborChildTopRight;
            _children.bottomLeft->_neighbors.left = neighborChildBottomRight;

            neighborChildTopRight->setNeighbor(_face, NeighborOrientation::RIGHT, _children.topLeft.get());
            neighborChildBottomRight->setNeighbor(_face, NeighborOrientation::RIGHT, _children.bottomLeft.get());
        }

        // Right neighbor
        if (_neighbors.right != nullptr && _neighbors.right->_split) {
            QuadTree* neighborChildTopLeft = _neighbors.right->getChild(_face, ChildOrientation::TOP_LEFT);
            QuadTree* neighborChildBottomLeft = _neighbors.right->getChild(_face, ChildOrientation::BOTTOM_LEFT);

            _children.topRight->_neighbors.right = neighborChildTopLeft;
            _children.bottomRight->_neighbors.right = neighborChildBottomLeft;

            neighborChildTopLeft->setNeighbor(_face, NeighborOrientation::LEFT, _children.topRight.get());
            neighborChildBottomLeft->setNeighbor(_face, NeighborOrientation::LEFT, _children.bottomRight.get());
        }

        // Top neighbor
        if (_neighbors.top != nullptr && _neighbors.top->_split) {
            QuadTree* neighborChildBottomLeft = _neighbors.top->getChild(_face, ChildOrientation::BOTTOM_LEFT);
            QuadTree* neighborChildBottomRight = _neighbors.top->getChild(_face, ChildOrientation::BOTTOM_RIGHT);

            _children.topLeft->_neighbors.top = neighborChildBottomLeft;
            _children.topRight->_neighbors.top = neighborChildBottomRight;

            neighborChildBottomLeft->setNeighbor(_face, NeighborOrientation::BOTTOM, _children.topLeft.get());
            neighborChildBottomRight->setNeighbor(_face, NeighborOrientation::BOTTOM, _children.topRight.get());
        }

        // Bottom neighbor
        if (_neighbors.bottom != nullptr && _neighbors.bottom->_split) {
            QuadTree* neighborChildTopLeft = _neighbors.bottom->getChild(_face, ChildOrientation::TOP_LEFT);
            QuadTree* neighborChildTopRight = _neighbors.bottom->getChild(_face, ChildOrientation::TOP_RIGHT);

            _children.bottomLeft->_neighbors.bottom = neighborChildTopLeft;
            _children.bottomRight->_neighbors.bottom = neighborChildTopRight;

            neighborChildTopLeft->setNeighbor(_face, NeighborOrientation::TOP, _children.bottomLeft.get());
            neighborChildTopRight->setNeighbor(_face, NeighborOrientation::TOP, _children.bottomRight.get());
        }
    }

    // Update neighbors link inside the quadtree (link all children)
    {
        _children.topLeft->_neighbors.right = _children.topRight.get();
        _children.topLeft->_neighbors.bottom = _children.bottomLeft.get();

        _children.topRight->_neighbors.left = _children.topLeft.get();
        _children.topRight->_neighbors.bottom = _children.bottomRight.get();

        _children.bottomLeft->_neighbors.right = _children.bottomRight.get();
        _children.bottomLeft->_neighbors.top = _children.topLeft.get();

        _children.bottomRight->_neighbors.left = _children.bottomLeft.get();
        _children.bottomRight->_neighbors.top = _children.topRight.get();
    }

}

void QuadTree::setNeighBors(QuadTree* top, QuadTree* left, QuadTree* right, QuadTree* bottom) {
    _neighbors.top = top;
    _neighbors.left = left;
    _neighbors.right = right;
    _neighbors.bottom = bottom;
}

/*
 * Returns the rotation needed to get an orientation from "QuadTree::_face space" to "fromFace space"
 * Rotation returned value is between 0 and 3
 *
 * Example:
 *
 * Top face:         Bottom face:
 * TL | TR           BL | TL
 * -------           -------
 * BL | BR           BR | TR
 *
 * if getOrientationRotationFromFace(Face::BOTTOM) is called on a top face QuadTree (with QuadTree::_face == Face::TOP)
 * it will return 1 because we need 1 rotation to go from "Top face space" to "Bottom face space"
 *
 * TODO: Put this in SphereQuadTree
*/
uint8_t QuadTree::getOrientationRotationFromFace(Face fromFace) {
    uint8_t orientationRotation = 0;

    if (fromFace == Face::RIGHT) {
        if (_face == Face::TOP) {
            orientationRotation = 3;
        }
        else if (_face == Face::BOTTOM) {
            orientationRotation = 1;
        }
    }
    else if (fromFace == Face::LEFT) {
        if (_face == Face::TOP) {
            orientationRotation = 1;
        }
        else if (_face == Face::BOTTOM) {
            orientationRotation = 3;
        }
    }
    else if (fromFace == Face::BACK) {
        if (_face == Face::TOP || _face == Face::BOTTOM) {
            orientationRotation = 2;
        }
    }
    else if (fromFace == Face::TOP) {
        if (_face == Face::RIGHT) {
            orientationRotation = 1;
        }
        else if (_face == Face::BACK) {
            orientationRotation = 2;
        }
        else if (_face == Face::LEFT) {
            orientationRotation = 3;
        }
    }
    else if (fromFace == Face::BOTTOM) {
        if (_face == Face::LEFT) {
            orientationRotation = 1;
        }
        else if (_face == Face::BACK) {
            orientationRotation = 2;
        }
        else if (_face == Face::RIGHT) {
            orientationRotation = 3;
        }
    }

    return orientationRotation;
}

QuadTree* QuadTree::getChild(Face fromFace, ChildOrientation childOrientation) {
    ChildOrientation orientation = childOrientation + getOrientationRotationFromFace(fromFace);

    if (orientation == ChildOrientation::TOP_LEFT) {
        return _children.topLeft.get();
    }
    else if (orientation == ChildOrientation::TOP_RIGHT) {
        return _children.topRight.get();
    }
    else if (orientation == ChildOrientation::BOTTOM_RIGHT) {
        return _children.bottomRight.get();
    }
    else if (orientation == ChildOrientation::BOTTOM_LEFT) {
        return _children.bottomLeft.get();
    }

    return nullptr;
}

QuadTree* QuadTree::getNeighbor(Face fromFace, NeighborOrientation neighborOrientation) {
    NeighborOrientation orientation = neighborOrientation + getOrientationRotationFromFace(fromFace);

    if (orientation == NeighborOrientation::TOP) {
        return _neighbors.top;
    }
    else if (orientation == NeighborOrientation::RIGHT) {
        return _neighbors.right;
    }
    else if (orientation == NeighborOrientation::BOTTOM) {
        return _neighbors.bottom;
    }
    else if (orientation == NeighborOrientation::LEFT) {
        return _neighbors.left;
    }

    return nullptr;
}

void QuadTree::setNeighbor(Face fromFace, NeighborOrientation neighborOrientation, QuadTree* neighbor) {
    NeighborOrientation orientation = neighborOrientation + getOrientationRotationFromFace(fromFace);

    if (orientation == NeighborOrientation::TOP) {
        _neighbors.top = neighbor;
    }
    else if (orientation == NeighborOrientation::RIGHT) {
        _neighbors.right = neighbor;
    }
    else if (orientation == NeighborOrientation::BOTTOM) {
        _neighbors.bottom = neighbor;
    }
    else if (orientation == NeighborOrientation::LEFT) {
        _neighbors.left = neighbor;
    }

}

void QuadTree::addChildrenVertices(System::Vector<Vertex>& vertices, System::Vector<uint32_t>& indices) {
        uint32_t verticesNb = static_cast<uint32_t>(vertices.size());

        /*
         * A quadtree with children should look like this
         * So we can remove adjacent vertices to prevent cracks
         *                   ___ ___
         *                  |\  |  /|
         * TL (top left)    | \ | / | TR (top right)
         *                  |__\|/__|
         *                  |  /|\  |
         * BL (bottom left) | / | \ | BR (bottom right)
         *                  |/__|__\|
         *
         *
        */


        /*  ___
         * |\  |
         * | \ |    TL or BR
         * |__\|
         *
        */
         uint32_t TLIndex = verticesNb;
         uint32_t BRIndex = verticesNb + 4;
         uint32_t BLIndex = BRIndex + 4;
         uint32_t TRIndex = BLIndex + 4;
        {
            // TL
            vertices.push_back(_children.topLeft->_corners[0]);
            vertices.push_back(_children.topLeft->_corners[1]);
            vertices.push_back(_children.topLeft->_corners[2]);
            vertices.push_back(_children.topLeft->_corners[3]);
            if (!_children.topLeft->_split) {
                if (_children.topLeft->_neighbors.left) {
                    indices.push_back(TLIndex);
                    indices.push_back(TLIndex + 2);
                    indices.push_back(TLIndex + 3);
                }

                if (_children.topLeft->_neighbors.top) {
                    indices.push_back(TLIndex);
                    indices.push_back(TLIndex + 3);
                    indices.push_back(TLIndex + 1);
                }

            }

            // BR
            vertices.push_back(_children.bottomRight->_corners[0]);
            vertices.push_back(_children.bottomRight->_corners[1]);
            vertices.push_back(_children.bottomRight->_corners[2]);
            vertices.push_back(_children.bottomRight->_corners[3]);
            if (!_children.bottomRight->_split) {
                if (_children.bottomRight->_neighbors.bottom) {
                    indices.push_back(BRIndex);
                    indices.push_back(BRIndex + 2);
                    indices.push_back(BRIndex + 3);
                }
                if (_children.bottomRight->_neighbors.right) {
                    indices.push_back(BRIndex);
                    indices.push_back(BRIndex + 3);
                    indices.push_back(BRIndex + 1);
                }
            }
        }

        /*  ___
         * |  /|
         * | / |    BL or TR
         * |/__|
         *
        */
        {
            // BL
            vertices.push_back(_children.bottomLeft->_corners[0]);
            vertices.push_back(_children.bottomLeft->_corners[1]);
            vertices.push_back(_children.bottomLeft->_corners[2]);
            vertices.push_back(_children.bottomLeft->_corners[3]);
            if (!_children.bottomLeft->_split) {
                if (_children.bottomLeft->_neighbors.left) {
                    indices.push_back(BLIndex);
                    indices.push_back(BLIndex + 2);
                    indices.push_back(BLIndex + 1);
                }
                if (_children.bottomLeft->_neighbors.top) {
                    indices.push_back(BLIndex + 1);
                    indices.push_back(BLIndex + 2);
                    indices.push_back(BLIndex + 3);
                }
            }

            // TR
            vertices.push_back(_children.topRight->_corners[0]);
            vertices.push_back(_children.topRight->_corners[1]);
            vertices.push_back(_children.topRight->_corners[2]);
            vertices.push_back(_children.topRight->_corners[3]);
            if (!_children.topRight->_split) {
                if (_children.topRight->_neighbors.top) {
                    indices.push_back(TRIndex);
                    indices.push_back(TRIndex + 2);
                    indices.push_back(TRIndex + 1);
                }
                if (_children.topRight->_neighbors.right) {
                    indices.push_back(TRIndex + 1);
                    indices.push_back(TRIndex + 2);
                    indices.push_back(TRIndex + 3);
                }
            }
        }

        // Fill gaps caused by removed vertices
        {
            if (!_children.topLeft->_split &&
                !_children.topRight->_split &&
                !_children.topLeft->_neighbors.top &&
                !_children.topRight->_neighbors.top) {
                indices.push_back(TLIndex);
                indices.push_back(TLIndex + 3);
                indices.push_back(TRIndex + 1);
            }
            if (!_children.topLeft->_split &&
                !_children.bottomLeft->_split &&
                !_children.topLeft->_neighbors.left &&
                !_children.bottomLeft->_neighbors.left) {
                indices.push_back(TLIndex);
                indices.push_back(BLIndex + 2);
                indices.push_back(TLIndex + 3);
            }
            if (!_children.topRight->_split &&
                !_children.bottomRight->_split &&
                !_children.topRight->_neighbors.right &&
                !_children.bottomRight->_neighbors.right) {
                indices.push_back(TRIndex + 1);
                indices.push_back(TRIndex + 2);
                indices.push_back(BRIndex + 3);
            }
            if (!_children.bottomLeft->_split &&
                !_children.bottomRight->_split &&
                !_children.bottomLeft->_neighbors.bottom &&
                !_children.bottomRight->_neighbors.bottom) {
                indices.push_back(BLIndex + 1);
                indices.push_back(BLIndex + 2);
                indices.push_back(BRIndex + 3);
            }
        }
}

bool QuadTree::needSplit(const Graphics::Camera& camera) {
    float distance = glm::distance(camera.getPos(), _center);

    return !_split &&
    _level < _levelsTable.size() &&
    distance < _levelsTable[_level];
}

void QuadTree::split() {
    float childrenSize = _size / 2;
    _children.topLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal,
        _face,
        _levelsTable,
        _level + 1,
        this
        );
    _children.topRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize) + (_heightDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal,
        _face,
        _levelsTable,
        _level + 1,
        this
        );
    _children.bottomLeft = std::make_unique<QuadTree>(
        childrenSize,
        _pos,
        _widthDir,
        _heightDir,
        _normal,
        _face,
        _levelsTable,
        _level + 1,
        this
        );
    _children.bottomRight = std::make_unique<QuadTree>(
        childrenSize,
        _pos + (_widthDir * childrenSize),
        _widthDir,
        _heightDir,
        _normal,
        _face,
        _levelsTable,
        _level + 1,
        this
        );

    _split = true;

    updateNeighBors();
}

bool QuadTree::needMerge(const Graphics::Camera& camera) {
    float distance = glm::distance(camera.getPos(), _center);

    return _split &&
    _level >= 0 &&
    distance > _levelsTable[_level];
}

void QuadTree::merge() {
    if (_children.topLeft->_split) {
        _children.topLeft->merge();
    }
    if (_children.topRight->_split) {
        _children.topRight->merge();
    }
    if (_children.bottomLeft->_split) {
        _children.bottomLeft->merge();
    }
    if (_children.bottomRight->_split) {
        _children.bottomRight->merge();
    }


    _split = false;

    updateNeighBors();

    _children.topLeft = nullptr;
    _children.topRight = nullptr;
    _children.bottomLeft = nullptr;
    _children.bottomRight = nullptr;
}

bool QuadTree::isInsideFrustrum(const Graphics::Camera& camera) const {
    return true;
}

} // Namespace Core

