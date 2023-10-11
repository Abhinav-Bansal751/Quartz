#pragma once

#include "util/logger/Logger.hpp"

DECLARE_LOGGER(VULKAN, trace);
DECLARE_LOGGER(INSTANCE, trace);
DECLARE_LOGGER(DEVICE, trace);
DECLARE_LOGGER(WINDOW, trace);
DECLARE_LOGGER(PIPELINE, trace);
DECLARE_LOGGER(SWAPCHAIN, trace);

DECLARE_LOGGER_GROUP(
        QUARTZ_RENDERING,
        6,
        VULKAN,
        INSTANCE,
        DEVICE,
        WINDOW,
        PIPELINE,
        SWAPCHAIN
);
