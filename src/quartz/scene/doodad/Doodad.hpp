#pragma once

#include <string>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "quartz/rendering/device/Device.hpp"
#include "quartz/rendering/model/Model.hpp"
#include "quartz/scene/Loggers.hpp"

namespace quartz {
namespace scene {
    class Doodad;
}
}

class quartz::scene::Doodad {
public: // member functions
    Doodad(
        const quartz::rendering::Device& renderingDevice,
        const std::string& objectFilepath,
        const std::string& textureFilepath,
        const glm::vec3& worldPosition
    );
    Doodad(Doodad&& other);
    ~Doodad();

    USE_LOGGER(DOODAD);

    const quartz::rendering::Model& getModel() const { return m_model; }
    const glm::mat4& getModelMatrix() const { return m_modelMatrix; }

    void update(const double tickTimeDelta);

private: // static functions

private: // member variables
    quartz::rendering::Model m_model;

    glm::vec3 m_worldPosition;

    glm::mat4 m_modelMatrix;
};