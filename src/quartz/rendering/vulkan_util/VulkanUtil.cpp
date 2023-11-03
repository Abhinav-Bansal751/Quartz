#include <vulkan/vulkan.hpp>

#include "util/logger/Logger.hpp"

#include "quartz/rendering/Loggers.hpp"
#include "quartz/rendering/vulkan_util/VulkanUtil.hpp"

vk::UniqueImageView
quartz::rendering::VulkanUtil::createVulkanImageViewPtr(
    const vk::UniqueDevice& p_logicalDevice,
    const vk::Image& image,
    const vk::Format format,
    const vk::ComponentMapping components,
    const vk::ImageAspectFlags imageAspectFlags
) {
    LOG_FUNCTION_SCOPE_TRACE(VULKANUTIL, "");

    vk::ImageViewCreateInfo imageViewCreateInfo(
        {},
        image,
        vk::ImageViewType::e2D,
        format,
        components,
        {
            imageAspectFlags,
            0,
            1,
            0,
            1
        }
    );

    vk::UniqueImageView p_imageView =
        p_logicalDevice->createImageViewUnique(imageViewCreateInfo);

    if (!p_imageView) {
        LOG_CRITICAL(VULKANUTIL, "Failed to create vk::ImageView");
        throw std::runtime_error("");
    }

    return p_imageView;
}


vk::UniqueCommandPool
quartz::rendering::VulkanUtil::createVulkanCommandPoolUniquePtr(
    const uint32_t graphicsQueueFamilyIndex,
    const vk::UniqueDevice& p_logicalDevice,
    const vk::CommandPoolCreateFlags flags
) {
    LOG_FUNCTION_SCOPE_TRACE(VULKANUTIL, "");

    vk::CommandPoolCreateInfo commandPoolCreateInfo(
        flags,
        graphicsQueueFamilyIndex
    );

    vk::UniqueCommandPool p_commandPool =
        p_logicalDevice->createCommandPoolUnique(commandPoolCreateInfo);

    if (!p_commandPool) {
        LOG_CRITICAL(VULKANUTIL, "Failed to create vk::CommandPool");
        throw std::runtime_error("");
    }

    return p_commandPool;
}