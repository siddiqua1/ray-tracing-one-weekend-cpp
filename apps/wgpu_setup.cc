#include "webgpu-utils.h"

// Include the C++ wrapper instead of the raw header(s)
#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu/webgpu.hpp>

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif // __EMSCRIPTEN__

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class Application {
public:
  // Initialize everything and return true if it went all right
  bool Initialize();

  // Uninitialize everything that was initialized
  void Terminate();

  // Draw a frame and handle events
  void MainLoop();

  // Return true as long as the main loop should keep on running
  bool IsRunning();

private:
  wgpu::TextureView GetNextSurfaceTextureView();

private:
  // We put here all the variables that are shared between init and main loop
  GLFWwindow *window;
  wgpu::Device device;
  wgpu::Queue queue;
  wgpu::Surface surface;
  std::unique_ptr<wgpu::ErrorCallback> uncapturedErrorCallbackHandle;
};

int main() {
  Application app;

  if (!app.Initialize()) {
    return 1;
  }

#ifdef __EMSCRIPTEN__
  // Equivalent of the main loop when using Emscripten:
  auto callback = [](void *arg) {
    Application *pApp = reinterpret_cast<Application *>(arg);
    pApp->MainLoop(); // 4. We can use the application object
  };
  emscripten_set_main_loop_arg(callback, &app, 0, true);
#else  // __EMSCRIPTEN__
  while (app.IsRunning()) {
    app.MainLoop();
  }
#endif // __EMSCRIPTEN__

  app.Terminate();

  return 0;
}

bool Application::Initialize() {
  // Open window
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(640, 480, "Learn WebGPU", nullptr, nullptr);

  wgpu::InstanceDescriptor desc = {};
  wgpu::Instance instance = wgpu::createInstance(desc);

  std::cout << "Requesting adapter..." << std::endl;
  surface = glfwGetWGPUSurface(instance, window);
  wgpu::RequestAdapterOptions adapterOpts = {};
  adapterOpts.nextInChain = nullptr;
  adapterOpts.compatibleSurface = surface;
  wgpu::Adapter adapter = requestAdapterSync(instance, &adapterOpts);
  std::cout << "Got adapter: " << adapter << std::endl;

  wgpuInstanceRelease(instance);

  std::cout << "Requesting device..." << std::endl;
  wgpu::DeviceDescriptor deviceDesc = {};
  deviceDesc.nextInChain = nullptr;
  deviceDesc.label = "My Device";
  deviceDesc.requiredFeatureCount = 0;
  deviceDesc.requiredLimits = nullptr;
  deviceDesc.defaultQueue.nextInChain = nullptr;
  deviceDesc.defaultQueue.label = "The default queue";
  deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason,
                                     char const *message,
                                     void * /* pUserData */) {
    std::cout << "Device lost: reason " << reason;
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };
  device = requestDeviceSync(adapter, &deviceDesc);
  std::cout << "Got device: " << device << std::endl;

  uncapturedErrorCallbackHandle = device.setUncapturedErrorCallback(
      [](wgpu::ErrorType type, char const *message) {
        std::cout << "Uncaptured device error: type " << type;
        if (message)
          std::cout << " (" << message << ")";
        std::cout << std::endl;
      });

  queue = device.getQueue();

  // Configure the surface
  wgpu::SurfaceConfiguration config = {};
  config.nextInChain = nullptr;

  // Configuration of the textures created for the underlying swap chain
  config.width = 640;
  config.height = 480;
  config.usage = wgpu::TextureUsage::RenderAttachment;
  wgpu::TextureFormat surfaceFormat =
      wgpuSurfaceGetPreferredFormat(surface, adapter);
  config.format = surfaceFormat;

  // And we do not need any particular view format:
  config.viewFormatCount = 0;
  config.viewFormats = nullptr;
  config.device = device;
  config.presentMode = wgpu::PresentMode::Fifo;
  config.alphaMode = wgpu::CompositeAlphaMode::Auto;

  wgpuSurfaceConfigure(surface, &config);

  // Release the adapter only after it has been fully utilized
  wgpuAdapterRelease(adapter);

  return true;
}

void Application::Terminate() {
  // Unconfigure the surface
  wgpuSurfaceUnconfigure(surface);
  wgpuQueueRelease(queue);
  wgpuSurfaceRelease(surface);
  wgpuDeviceRelease(device);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Application::MainLoop() {
  glfwPollEvents();

  // Get the next target texture view
  wgpu::TextureView targetView = GetNextSurfaceTextureView();
  if (!targetView)
    return;

  // Create a command encoder for the draw call
  wgpu::CommandEncoderDescriptor encoderDesc = {};
  encoderDesc.nextInChain = nullptr;
  encoderDesc.label = "My command encoder";
  wgpu::CommandEncoder encoder =
      wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

  // Create the render pass that clears the screen with our color
  wgpu::RenderPassDescriptor renderPassDesc = {};
  renderPassDesc.nextInChain = nullptr;

  // The attachment part of the render pass descriptor describes the target
  // texture of the pass
  wgpu::RenderPassColorAttachment renderPassColorAttachment = {};
  renderPassColorAttachment.view = targetView;
  renderPassColorAttachment.resolveTarget = nullptr;
  renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
  renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
  renderPassColorAttachment.clearValue = wgpu::Color{0.9, 0.1, 0.2, 1.0};
#ifndef WEBGPU_BACKEND_WGPU
  renderPassColorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif // NOT WEBGPU_BACKEND_WGPU

  renderPassDesc.colorAttachmentCount = 1;
  renderPassDesc.colorAttachments = &renderPassColorAttachment;
  renderPassDesc.depthStencilAttachment = nullptr;
  renderPassDesc.timestampWrites = nullptr;

  // Create the render pass and end it immediately (we only clear the screen but
  // do not draw anything)
  wgpu::RenderPassEncoder renderPass =
      wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
  wgpuRenderPassEncoderEnd(renderPass);
  wgpuRenderPassEncoderRelease(renderPass);

  // Finally encode and submit the render pass
  wgpu::CommandBufferDescriptor cmdBufferDescriptor = {};
  cmdBufferDescriptor.nextInChain = nullptr;
  cmdBufferDescriptor.label = "Command buffer";
  wgpu::CommandBuffer command =
      wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
  wgpuCommandEncoderRelease(encoder);

  std::cout << "Submitting command..." << std::endl;
  queue.submit(command);
  command.release();
  std::cout << "Command submitted." << std::endl;

  // At the end of the frame
  wgpuTextureViewRelease(targetView);
#ifndef __EMSCRIPTEN__
  wgpuSurfacePresent(surface);
#endif

#if defined(WEBGPU_BACKEND_DAWN)
  wgpuDeviceTick(device);
#elif defined(WEBGPU_BACKEND_WGPU)
  wgpuDevicePoll(device, false, nullptr);
#endif
}

bool Application::IsRunning() { return !glfwWindowShouldClose(window); }

wgpu::TextureView Application::GetNextSurfaceTextureView() {
  // Get the surface texture
  wgpu::SurfaceTexture surfaceTexture;
  surface.getCurrentTexture(&surfaceTexture);
  if (surfaceTexture.status != wgpu::SurfaceGetCurrentTextureStatus::Success) {
    return nullptr;
  }

  // Create a view for this surface texture
  wgpu::TextureViewDescriptor viewDescriptor;
  viewDescriptor.nextInChain = nullptr;
  viewDescriptor.label = "Surface texture view";
  viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
  viewDescriptor.dimension = wgpu::TextureViewDimension::_2D;
  viewDescriptor.baseMipLevel = 0;
  viewDescriptor.mipLevelCount = 1;
  viewDescriptor.baseArrayLayer = 0;
  viewDescriptor.arrayLayerCount = 1;
  viewDescriptor.aspect = wgpu::TextureAspect::All;
  wgpu::TextureView targetView =
      wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

#ifndef WEBGPU_BACKEND_WGPU
  // We no longer need the texture, only its view
  // (NB: with wgpu-native, surface textures must not be manually released)
  wgpuTextureRelease(surfaceTexture.texture);
#endif // WEBGPU_BACKEND_WGPU

  return targetView;
}
