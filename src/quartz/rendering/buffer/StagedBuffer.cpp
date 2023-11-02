#include <vulkan/vulkan.hpp>

#include "quartz/rendering/Loggers.hpp"
#include "quartz/rendering/buffer/BufferHelper.hpp"
#include "quartz/rendering/buffer/StagedBuffer.hpp"

vk::UniqueDeviceMemory
quartz::rendering::StagedBuffer::allocateVulkanPhysicalDeviceDestinationMemoryUniquePtr(
    const vk::PhysicalDevice& physicalDevice,
    const uint32_t graphicsQueueFamilyIndex,
    const vk::UniqueDevice& p_logicalDevice,
    const vk::Queue& graphicsQueue,
    const uint32_t sizeBytes,
    const vk::UniqueBuffer& p_logicalBuffer,
    const vk::MemoryPropertyFlags requiredMemoryProperties,
    const vk::UniqueBuffer& p_logicalStagingBuffer
) {
    LOG_FUNCTION_SCOPE_TRACE(BUFFER, "{} bytes", sizeBytes);
    
    vk::UniqueDeviceMemory p_logicalBufferPhysicalMemory =
        quartz::rendering::BufferHelper::allocateVulkanPhysicalDeviceMemoryUniquePtr(
            physicalDevice,
            p_logicalDevice,
            sizeBytes,
            p_logicalBuffer,
            requiredMemoryProperties
        );

    LOG_TRACE(
        BUFFER,
        "Memory is *NOT* allocated for a source buffer. Populating with input "
        "source buffer instead"
    );

    LOG_TRACE(BUFFER, "Attempting to create vk::CommandPool");

    vk::CommandPoolCreateInfo commandPoolCreateInfo(
        vk::CommandPoolCreateFlagBits::eTransient,
        graphicsQueueFamilyIndex
    );

    vk::UniqueCommandPool p_commandPool =
        p_logicalDevice->createCommandPoolUnique(
            commandPoolCreateInfo
        );

    if (!p_commandPool) {
        LOG_CRITICAL(BUFFER, "Failed to create vk::CommandPool");
        throw std::runtime_error("");
    }
    LOG_TRACE(BUFFER, "Successfully created vk::CommandPool");

    LOG_TRACE(BUFFER, "Attempting to allocate vk::CommandBuffer");

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo(
        *p_commandPool,
        vk::CommandBufferLevel::ePrimary,
        1
    );

    std::vector<vk::UniqueCommandBuffer> commandBufferPtrs =
        p_logicalDevice->allocateCommandBuffersUnique(
            commandBufferAllocateInfo
        );

    if (!(commandBufferPtrs[0])) {
        LOG_CRITICAL(BUFFER, "Failed to allocate vk::CommandBuffer");
        throw std::runtime_error("");
    }

    LOG_TRACE(BUFFER, "Recording commands to newly created command buffer");

    vk::CommandBufferBeginInfo commandBufferBeginInfo(
        vk::CommandBufferUsageFlagBits::eOneTimeSubmit
    );
    commandBufferPtrs[0]->begin(commandBufferBeginInfo);

    vk::BufferCopy bufferCopyRegion(
        0,
        0,
        sizeBytes
    );

    commandBufferPtrs[0]->copyBuffer(
        *p_logicalStagingBuffer,
        *p_logicalBuffer,
        bufferCopyRegion
    );

    commandBufferPtrs[0]->end();

    LOG_TRACE(
        BUFFER,
        "Submitting command buffer and waiting idly for it to complete the "
        "copying of data from staging buffer into this buffer"
    );

    vk::SubmitInfo submitInfo(
        0,
        nullptr,
        nullptr,
        1,
        &(*(commandBufferPtrs[0])),
        0,
        nullptr
    );
    graphicsQueue.submit(submitInfo, VK_NULL_HANDLE);
    graphicsQueue.waitIdle();

    LOG_TRACE(
        BUFFER,
        "Successfully copied data from staging buffer into this buffer's memory"
    );

    return p_logicalBufferPhysicalMemory;
}

quartz::rendering::StagedBuffer::StagedBuffer(
    const quartz::rendering::Device& renderingDevice,
    const uint32_t sizeBytes,
    const vk::BufferUsageFlags usageFlags,
    const void* p_bufferData
) :
    m_sizeBytes(sizeBytes),
    m_usageFlags(usageFlags),
    mp_vulkanLogicalStagingBuffer(
        quartz::rendering::BufferHelper::createVulkanBufferUniquePtr(
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            vk::BufferUsageFlagBits::eTransferSrc
        )
    ),
    mp_vulkanPhysicalDeviceStagingMemory(
        quartz::rendering::BufferHelper::allocateVulkanPhysicalDeviceStagingMemoryUniquePtr(
            renderingDevice.getVulkanPhysicalDevice(),
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            p_bufferData,
            mp_vulkanLogicalStagingBuffer,
            {
                vk::MemoryPropertyFlagBits::eHostVisible |
                vk::MemoryPropertyFlagBits::eHostCoherent
            }
        )
    ),
    mp_vulkanLogicalBuffer(
        quartz::rendering::BufferHelper::createVulkanBufferUniquePtr(
            renderingDevice.getVulkanLogicalDevicePtr(),
            m_sizeBytes,
            vk::BufferUsageFlagBits::eTransferDst | m_usageFlags
        )
    ),
    mp_vulkanPhysicalDeviceMemory(
        quartz::rendering::StagedBuffer::allocateVulkanPhysicalDeviceDestinationMemoryUniquePtr(
            renderingDevice.getVulkanPhysicalDevice(),
            renderingDevice.getGraphicsQueueFamilyIndex(),
            renderingDevice.getVulkanLogicalDevicePtr(),
            renderingDevice.getVulkanGraphicsQueue(),
            m_sizeBytes,
            mp_vulkanLogicalBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            mp_vulkanLogicalStagingBuffer
        )
    )
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::rendering::StagedBuffer::StagedBuffer(
    quartz::rendering::StagedBuffer&& other
) :
    m_sizeBytes(
        other.m_sizeBytes
    ),
    m_usageFlags(
        other.m_usageFlags
    ),
    mp_vulkanLogicalStagingBuffer(std::move(
        other.mp_vulkanLogicalStagingBuffer
    )),
    mp_vulkanPhysicalDeviceStagingMemory(std::move(
        other.mp_vulkanPhysicalDeviceStagingMemory
    )),
    mp_vulkanLogicalBuffer(std::move(
        other.mp_vulkanLogicalBuffer
    )),
    mp_vulkanPhysicalDeviceMemory(std::move(
        other.mp_vulkanPhysicalDeviceMemory
    ))
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::rendering::StagedBuffer::~StagedBuffer() {
    LOG_FUNCTION_CALL_TRACEthis("");
}
