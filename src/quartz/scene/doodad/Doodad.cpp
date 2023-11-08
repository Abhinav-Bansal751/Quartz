#include <string>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "quartz/scene/doodad/Doodad.hpp"

quartz::scene::Doodad::Doodad(
    const quartz::rendering::Device& renderingDevice,
    const std::string& objectFilepath,
    const std::string& textureFilepath,
    const glm::vec3& worldPosition
) :
    m_model(
        renderingDevice,
        objectFilepath,
        textureFilepath
    ),
    m_worldPosition(worldPosition),
    m_modelMatrix()
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::scene::Doodad::Doodad(
    quartz::scene::Doodad&& other
) :
    m_model(std::move(other.m_model)),
    m_modelMatrix(other.m_modelMatrix)
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::scene::Doodad::~Doodad() {
    LOG_FUNCTION_CALL_TRACEthis("");
}

void
quartz::scene::Doodad::update(
    UNUSED const double tickTimeDelta
) {
    m_modelMatrix = glm::mat4(1.0f);

    m_modelMatrix = glm::translate(
        m_modelMatrix,
        m_worldPosition
    );

//    m_modelMatrix = glm::rotate(
//        m_modelMatrix,
//        executionDurationTimeCount * glm::radians(90.0f) * 0.0f,
//        glm::vec3(0.0f, 0.0f, 1.0f)
//    );
}