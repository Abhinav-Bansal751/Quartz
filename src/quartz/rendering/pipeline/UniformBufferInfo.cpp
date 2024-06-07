#include "quartz/rendering/pipeline/UniformBufferInfo.hpp"

#include "util/macros.hpp"

uint32_t
quartz::rendering::UniformBufferInfo::calculateDynamicUniformBufferByteStride(
    const quartz::rendering::Device &renderingDevice,
    const uint32_t uniformBufferObjectSizeBytes
) {
    const uint32_t minUniformBufferOffsetAlignment = renderingDevice.getVulkanPhysicalDevice().getProperties().limits.minUniformBufferOffsetAlignment;

    const uint32_t byteStride = minUniformBufferOffsetAlignment > 0 ?
        (uniformBufferObjectSizeBytes + minUniformBufferOffsetAlignment - 1) & ~(minUniformBufferOffsetAlignment - 1) :
        uniformBufferObjectSizeBytes;

    return byteStride;
}

quartz::rendering::UniformBufferInfo::UniformBufferInfo(
    UNUSED const quartz::rendering::Device& renderingDevice,
    const uint32_t locallyMappedBufferSizeBytes,
    const vk::MemoryPropertyFlags locallyMappedBufferPropertyFlags,
    const uint32_t bindingLocation,
    const uint32_t descriptorCount,
    const uint32_t objectStrideBytes,
    const vk::DescriptorType descriptorType,
    const vk::ShaderStageFlags shaderStageFlags
) :
    m_locallyMappedBufferSize(locallyMappedBufferSizeBytes),
    m_locallyMappedBufferUsageFlags(vk::BufferUsageFlagBits::eUniformBuffer),
    m_locallyMappedBufferPropertyFlags(locallyMappedBufferPropertyFlags),
    m_bindingLocation(bindingLocation),
    m_descriptorCount(descriptorCount),
    m_objectStrideBytes(objectStrideBytes),
    m_vulkanDescriptorType(descriptorType),
    m_vulkanShaderStageFlags(shaderStageFlags)
{}

quartz::rendering::UniformBufferInfo::UniformBufferInfo(
    const quartz::rendering::UniformBufferInfo& other
) :
    m_locallyMappedBufferSize(other.m_locallyMappedBufferSize),
    m_locallyMappedBufferUsageFlags(other.m_locallyMappedBufferUsageFlags),
    m_locallyMappedBufferPropertyFlags(other.m_locallyMappedBufferPropertyFlags),
    m_bindingLocation(other.m_bindingLocation),
    m_descriptorCount(other.m_descriptorCount),
    m_objectStrideBytes(other.m_objectStrideBytes),
    m_vulkanDescriptorType(other.m_vulkanDescriptorType),
    m_vulkanShaderStageFlags(other.m_vulkanShaderStageFlags)
{}

quartz::rendering::UniformBufferInfo::UniformBufferInfo(
    quartz::rendering::UniformBufferInfo&& other
) :
    m_locallyMappedBufferSize(other.m_locallyMappedBufferSize),
    m_locallyMappedBufferUsageFlags(other.m_locallyMappedBufferUsageFlags),
    m_locallyMappedBufferPropertyFlags(other.m_locallyMappedBufferPropertyFlags),
    m_bindingLocation(other.m_bindingLocation),
    m_descriptorCount(other.m_descriptorCount),
    m_objectStrideBytes(other.m_objectStrideBytes),
    m_vulkanDescriptorType(other.m_vulkanDescriptorType),
    m_vulkanShaderStageFlags(other.m_vulkanShaderStageFlags)
{}

quartz::rendering::UniformBufferInfo::~UniformBufferInfo() {}

quartz::rendering::UniformBufferInfo&
quartz::rendering::UniformBufferInfo::operator=(
    const quartz::rendering::UniformBufferInfo& other
) {
    if (this == &other) {
        return *this;
    }

    m_locallyMappedBufferSize = other.m_locallyMappedBufferSize;
    m_locallyMappedBufferUsageFlags = other.m_locallyMappedBufferUsageFlags;
    m_locallyMappedBufferPropertyFlags = other.m_locallyMappedBufferPropertyFlags;
    m_bindingLocation = other.m_bindingLocation;
    m_descriptorCount = other.m_descriptorCount;
    m_objectStrideBytes = other.m_objectStrideBytes;
    m_vulkanDescriptorType = other.m_vulkanDescriptorType;
    m_vulkanShaderStageFlags = other.m_vulkanShaderStageFlags;

    return *this;
}

quartz::rendering::UniformBufferInfo&
quartz::rendering::UniformBufferInfo::operator=(
    quartz::rendering::UniformBufferInfo&& other
) {
    if (this == &other) {
        return *this;
    }

    m_locallyMappedBufferSize = other.m_locallyMappedBufferSize;
    m_locallyMappedBufferUsageFlags = other.m_locallyMappedBufferUsageFlags;
    m_locallyMappedBufferPropertyFlags = other.m_locallyMappedBufferPropertyFlags;
    m_bindingLocation = other.m_bindingLocation;
    m_descriptorCount = other.m_descriptorCount;
    m_objectStrideBytes = other.m_objectStrideBytes;
    m_vulkanDescriptorType = other.m_vulkanDescriptorType;
    m_vulkanShaderStageFlags = other.m_vulkanShaderStageFlags;

    return *this;
}