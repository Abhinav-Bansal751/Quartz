#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <vulkan/vulkan.hpp>

namespace quartz {
namespace rendering {
    struct Vertex;
}
}

struct quartz::rendering::Vertex {
public: // member functions
    Vertex(
        const glm::vec3& worldPosition_,
        const glm::vec3& color_,
        const glm::vec2& textureCoordinate_
    );

public: // static functions
    static vk::VertexInputBindingDescription getVulkanVertexInputBindingDescription();
    static std::array<vk::VertexInputAttributeDescription, 3> getVulkanVertexInputAttributeDescriptions();

public: // member variables
    glm::vec3 worldPosition;
    glm::vec3 color;
    glm::vec2 textureCoordinate;
};
