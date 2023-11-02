#include <vulkan/vulkan.hpp>

#include "quartz/rendering/Loggers.hpp"
#include "quartz/rendering/buffer/BufferHelper.hpp"
#include "quartz/rendering/buffer/LocallyMappedBuffer.hpp"

void*
quartz::rendering::LocallyMappedBuffer::mapVulkanPhysicalDeviceMemoryToLocalMemory(
    const vk::UniqueDevice& p_logicalDevice,
    const uint32_t sizeBytes,
    const vk::UniqueDeviceMemory& p_physicalDeviceMemory
) {
    LOG_FUNCTION_SCOPE_TRACE(BUFFER, "");

    LOG_TRACE(BUFFER, "Mapping memory from physical device memory to logical memory instance {}",
        static_cast<const void*>(&(*p_physicalDeviceMemory))
    );
    void* p_mappedLocalMemory = p_logicalDevice->mapMemory(
        *(p_physicalDeviceMemory),
        0,
        sizeBytes
    );

    LOG_TRACE(BUFFER, "Mapped to {}", p_mappedLocalMemory);
    return p_mappedLocalMemory;
}

quartz::rendering::LocallyMappedBuffer::LocallyMappedBuffer(
    const quartz::rendering::Device& renderingDevice,
    const uint32_t sizeBytes,
    const vk::BufferUsageFlags usageFlags,
    const vk::MemoryPropertyFlags memoryPropertyFlags
) :
    m_sizeBytes(sizeBytes),
    m_usageFlags(usageFlags),
    m_memoryPropertyFlags(memoryPropertyFlags),
    mp_vulkanLogicalBuffer(
        quartz::rendering::BufferHelper::createVulkanBufferUniquePtr(
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            m_usageFlags
        )
    ),
    mp_vulkanPhysicalDeviceMemory(
        quartz::rendering::BufferHelper::allocateVulkanPhysicalDeviceMemoryUniquePtr(
            renderingDevice.getVulkanPhysicalDevice(),
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            mp_vulkanLogicalBuffer,
            memoryPropertyFlags
        )
    ),
    mp_mappedLocalMemory(
        quartz::rendering::LocallyMappedBuffer::mapVulkanPhysicalDeviceMemoryToLocalMemory(
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            mp_vulkanPhysicalDeviceMemory
        )
    )
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::rendering::LocallyMappedBuffer::LocallyMappedBuffer(
    quartz::rendering::LocallyMappedBuffer&& other
) :
    m_sizeBytes(
        other.m_sizeBytes
    ),
    m_usageFlags(
        other.m_usageFlags
    ),
    m_memoryPropertyFlags(
        other.m_memoryPropertyFlags
    ),
    mp_vulkanLogicalBuffer(std::move(
        other.mp_vulkanLogicalBuffer
    )),
    mp_vulkanPhysicalDeviceMemory(std::move(
        other.mp_vulkanPhysicalDeviceMemory
    )),
    mp_mappedLocalMemory(std::move(
        other.mp_mappedLocalMemory
    ))
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::rendering::LocallyMappedBuffer::~LocallyMappedBuffer() {
    LOG_FUNCTION_CALL_TRACEthis("");
}