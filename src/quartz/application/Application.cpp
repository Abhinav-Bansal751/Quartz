#include <string>
#include <vector>

#include "util/file_system/FileSystem.hpp"

#include "quartz/Loggers.hpp"

#include "quartz/application/Application.hpp"

#include "quartz/rendering/device/Device.hpp"
#include "quartz/rendering/instance/Instance.hpp"
#include "quartz/rendering/mesh/Mesh.hpp"
#include "quartz/rendering/pipeline/Pipeline.hpp"
#include "quartz/rendering/swapchain/Swapchain.hpp"
#include "quartz/rendering/window/Window.hpp"

std::vector<quartz::rendering::Model>
quartz::Application::loadModels(
    const quartz::rendering::Device& renderingDevice
) {
    LOG_FUNCTION_SCOPE_TRACE(APPLICATION, "");

    std::vector<quartz::rendering::Model> models;
    models.emplace_back(
        renderingDevice,
        util::FileSystem::getAbsoluteFilepathInProject(
            "viking_room.obj"
        ),
        util::FileSystem::getAbsoluteFilepathInProject(
            "viking_room.png"
        )
    );
    LOG_TRACE(APPLICATION, "Loaded {} models", models.size());

    return models;
}

quartz::Application::Application(
    const std::string& applicationName,
    const uint32_t applicationMajorVersion,
    const uint32_t applicationMinorVersion,
    const uint32_t applicationPatchVersion,
    const uint32_t windowWidthPixels,
    const uint32_t windowHeightPixels,
    const bool validationLayersEnabled
) :
    m_applicationName(applicationName),
    m_majorVersion(applicationMajorVersion),
    m_minorVersion(applicationMinorVersion),
    m_patchVersion(applicationPatchVersion),
    m_renderingContext(
        m_applicationName,
        m_majorVersion,
        m_minorVersion,
        m_patchVersion,
        windowWidthPixels,
        windowHeightPixels,
        validationLayersEnabled
    ),
    mp_inputManager(quartz::managers::InputManager::getPtr(
        m_renderingContext.getRenderingWindow().getGLFWwindowPtr()
    )),
    m_models(),
    m_shouldQuit(false),
    m_isPaused(false)
{
    LOG_FUNCTION_CALL_TRACEthis("");
}

quartz::Application::~Application() {
    LOG_FUNCTION_CALL_TRACEthis("");
}

void quartz::Application::run() {
    LOG_FUNCTION_SCOPE_INFOthis("");

    LOG_INFOthis("Loading scene");
    m_models = quartz::Application::loadModels(
        m_renderingContext.getRenderingDevice()
    );
    m_renderingContext.loadScene(m_models);

    LOG_INFOthis("Beginning main loop");
    while(!m_shouldQuit) {
        // Capture input
        processInput();

        // Simulate the game world

        // Draw the game world
        draw();
    }

    LOG_INFOthis("Finishing");
    m_renderingContext.finish();
}

void
quartz::Application::processInput() {
    mp_inputManager->collectInput();

    m_shouldQuit =
        m_renderingContext.getRenderingWindow().shouldClose() ||
        mp_inputManager->getKeypressed_q();

    if (mp_inputManager->getKeypressed_esc()) {
        m_isPaused = !m_isPaused;

        LOG_DEBUGthis("{}ausing", (m_isPaused ? "P" : "Unp"));

        m_renderingContext.getRenderingWindow().setShouldDisplayCursor(m_isPaused);
        mp_inputManager->setShouldCollectMouseInput(m_isPaused);
    }
}

void
quartz::Application::draw() {
    m_renderingContext.draw(m_models);
}