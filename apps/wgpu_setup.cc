#include <webgpu/webgpu.h>

#include <cassert>
#include <iostream>
#include <optional>
#include <vector>
/**---------------------------------------------------------------------------*/
/** Forward Declarations */

WGPUAdapter requestAdapterSync(WGPUInstance instance,
                               WGPURequestAdapterOptions const *options);

void inspectAdapter(WGPUAdapter adapter);

/**
 * Utility function to get a WebGPU device, so that
 *     WGPUAdapter device = requestDeviceSync(adapter, options);
 * is roughly equivalent to
 *     const device = await adapter.requestDevice(descriptor);
 * It is very similar to requestAdapter
 */
WGPUDevice requestDeviceSync(WGPUAdapter adapter,
                             WGPUDeviceDescriptor const *descriptor);

/**
 * Display information about a device
 */
void inspectDevice(WGPUDevice device);

std::optional<WGPUDevice> get_device();
/**---------------------------------------------------------------------------*/
/** Main */
int main(int, char **) {
  auto maybe_device = get_device();
  if (!maybe_device.has_value()) {
    return 1;
  }
  auto device = maybe_device.value();
  WGPUQueue queue = wgpuDeviceGetQueue(device);
  auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status,
                            void * /* pUserData */) {
    std::cout << "Queued work finished with status: " << status << std::endl;
  };
  wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr /* pUserData */);

  wgpuDeviceRelease(device);

  return 0;
}
/**---------------------------------------------------------------------------*/
/** Function definitions */
WGPUAdapter requestAdapterSync(WGPUInstance instance,
                               WGPURequestAdapterOptions const *options) {
  struct UserData {
    WGPUAdapter adapter = nullptr;
    bool requestEnded = false;
  };
  UserData user_data;

  // define our callback
  auto on_adapter_request_ended = [](WGPURequestAdapterStatus status,
                                     WGPUAdapter adapter, char const *message,
                                     void *pUserData) {
    UserData &user_data = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestAdapterStatus_Success) {
      user_data.adapter = adapter;
    } else {
      std::cout << "Could not get WebGPU adapter: " << message << std::endl;
    }
    user_data.requestEnded = true;
  };

  wgpuInstanceRequestAdapter(instance, options, on_adapter_request_ended,
                             (void *)&user_data);

#ifdef __EMSCRIPTEN__
  while (!userData.requestEnded) {
    emscripten_sleep(100);
  }
#endif // __EMSCRIPTEN__

  assert(user_data.requestEnded);

  return user_data.adapter;
}

void inspectAdapter(WGPUAdapter adapter) {
#ifndef __EMSCRIPTEN__
  WGPUSupportedLimits supportedLimits = {};
  supportedLimits.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_DAWN
  bool success =
      wgpuAdapterGetLimits(adapter, &supportedLimits) == WGPUStatus_Success;
#else
  bool success = wgpuAdapterGetLimits(adapter, &supportedLimits);
#endif

  if (success) {
    std::cout << "Adapter limits:" << std::endl;
    std::cout << " - maxTextureDimension1D: "
              << supportedLimits.limits.maxTextureDimension1D << std::endl;
    std::cout << " - maxTextureDimension2D: "
              << supportedLimits.limits.maxTextureDimension2D << std::endl;
    std::cout << " - maxTextureDimension3D: "
              << supportedLimits.limits.maxTextureDimension3D << std::endl;
    std::cout << " - maxTextureArrayLayers: "
              << supportedLimits.limits.maxTextureArrayLayers << std::endl;
  }
#endif // NOT __EMSCRIPTEN__

  std::vector<WGPUFeatureName> features;

  // first call with nullptr gets us the count
  size_t feature_count = wgpuAdapterEnumerateFeatures(adapter, nullptr);

  // use count to allocate region which features would be populated
  features.resize(feature_count);

  // second call, provide the writeable region
  wgpuAdapterEnumerateFeatures(adapter, features.data());

  std::cout << "Adapter feature:" << std::endl;
  std::cout << std::hex;
  for (auto f : features) { // enum so cheap to copy
    std::cout << " - 0x" << f << std::endl;
  }
  std::cout << std::dec;

  WGPUAdapterProperties properties = {};
  properties.nextInChain = nullptr;
  wgpuAdapterGetProperties(adapter, &properties);
  std::cout << "Adatper properties:" << std::endl;
  std::cout << " - vendorID: " << properties.vendorID << std::endl;
  if (properties.vendorName) {
    std::cout << " - vendorName: " << properties.vendorName << std::endl;
  }
  if (properties.architecture) {
    std::cout << " - architecture: " << properties.architecture << std::endl;
  }
  std::cout << " - deviceID: " << properties.deviceID << std::endl;
  if (properties.name) {
    std::cout << " - name: " << properties.name << std::endl;
  }

  if (properties.driverDescription) {
    std::cout << " - driverDescription: " << properties.driverDescription
              << std::endl;
  }

  std::cout << std::hex;
  std::cout << " - adapterType: 0x" << properties.adapterType << std::endl;
  std::cout << " - backendType: 0x" << properties.backendType << std::endl;
  std::cout << std::dec;
}

WGPUDevice requestDeviceSync(WGPUAdapter adapter,
                             WGPUDeviceDescriptor const *descriptor) {
  struct UserData {
    WGPUDevice device = nullptr;
    bool requestEnded = false;
  };
  UserData userData;

  auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                 WGPUDevice device, char const *message,
                                 void *pUserData) {
    UserData &userData = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestDeviceStatus_Success) {
      userData.device = device;
    } else {
      std::cout << "Could not get WebGPU device: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  wgpuAdapterRequestDevice(adapter, descriptor, onDeviceRequestEnded,
                           (void *)&userData);

#ifdef __EMSCRIPTEN__
  while (!userData.requestEnded) {
    emscripten_sleep(100);
  }
#endif // __EMSCRIPTEN__

  assert(userData.requestEnded);

  return userData.device;
}

// We also add an inspect device function:
void inspectDevice(WGPUDevice device) {
  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuDeviceEnumerateFeatures(device, nullptr);
  features.resize(featureCount);
  wgpuDeviceEnumerateFeatures(device, features.data());

  std::cout << "Device features:" << std::endl;
  std::cout << std::hex;
  for (auto f : features) {
    std::cout << " - 0x" << f << std::endl;
  }
  std::cout << std::dec;

  WGPUSupportedLimits limits = {};
  limits.nextInChain = nullptr;

#ifdef WEBGPU_BACKEND_DAWN
  bool success = wgpuDeviceGetLimits(device, &limits) == WGPUStatus_Success;
#else
  bool success = wgpuDeviceGetLimits(device, &limits);
#endif

  if (success) {
    std::cout << "Device limits:" << std::endl;
    std::cout << " - maxTextureDimension1D: "
              << limits.limits.maxTextureDimension1D << std::endl;
    std::cout << " - maxTextureDimension2D: "
              << limits.limits.maxTextureDimension2D << std::endl;
    std::cout << " - maxTextureDimension3D: "
              << limits.limits.maxTextureDimension3D << std::endl;
    std::cout << " - maxTextureArrayLayers: "
              << limits.limits.maxTextureArrayLayers << std::endl;
  }
}

std::optional<WGPUDevice> get_device() {
  // We create a descriptor
  WGPUInstanceDescriptor desc = {};
  desc.nextInChain = nullptr;

  // We create the instance using this descriptor
#ifdef WEBGPU_BACKEND_EMSCRIPTEN
  WGPUInstance instance = wgpuCreateInstance(nullptr);
#else  //  WEBGPU_BACKEND_EMSCRIPTEN
  WGPUInstance instance = wgpuCreateInstance(&desc);
#endif //  WEBGPU_BACKEND_EMSCRIPTEN

  // We can check whether there is actually an instance created
  if (!instance) {
    std::cerr << "Could not initialize WebGPU!" << std::endl;
    return std::nullopt;
  }

  // Display the object (WGPUInstance is a simple pointer, it may be
  // copied around without worrying about its size).
  std::cout << "WGPU instance: " << instance << std::endl;

  std::cout << "Requesting adapter..." << std::endl;

  WGPURequestAdapterOptions adapterOpts = {};
  adapterOpts.nextInChain = nullptr;
  WGPUAdapter adapter = requestAdapterSync(instance, &adapterOpts);
  // no longer needed after we acquire the adapter
  wgpuInstanceRelease(instance);

  std::cout << "Got adapter: " << adapter << std::endl;

  inspectAdapter(adapter);

  std::cout << "Requesting device..." << std::endl;

  WGPUDeviceDescriptor deviceDesc = {};
  deviceDesc.nextInChain = nullptr;
  deviceDesc.label = "My Device";      // anything works here, that's your call
  deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
  deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
  deviceDesc.defaultQueue.nextInChain = nullptr;
  deviceDesc.defaultQueue.label = "The default queue";
  // Null for now, see below
  deviceDesc.deviceLostCallback = [](WGPUDeviceLostReason reason,
                                     char const *message,
                                     void * /* pUserData */) {
    std::cout << "Device lost: reason " << reason;
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };

  WGPUDevice device = requestDeviceSync(adapter, &deviceDesc);
  // should release adapter after we get out device
  wgpuAdapterRelease(adapter);

  // A function that is invoked whenever there is an error in the use of the
  // device
  auto onDeviceError = [](WGPUErrorType type, char const *message,
                          void * /* pUserData */) {
    std::cout << "Uncaptured device error: type " << type;
    if (message)
      std::cout << " (" << message << ")";
    std::cout << std::endl;
  };
  wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError,
                                       nullptr /* pUserData */);

  std::cout << "Got device: " << device << std::endl;
  inspectDevice(device);

  return device;
}