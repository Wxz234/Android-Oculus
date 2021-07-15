//
// Created by wyz on 2021/7/14.
//
#define XR_USE_PLATFORM_ANDROID
#define XR_USE_GRAPHICS_API_OPENGL_ES
#include <vr.h>
#include <jni.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <android_native_app_glue.h>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

void CHECK_XRCMD(XrResult xr){
    if(xr!=XR_SUCCESS){
        throw;
    }
}

#include <android/log.h>
#define LOGI__(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW__(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

std::string GetXrVersionString(XrVersion ver) {
    return std::to_string(XR_VERSION_MAJOR(ver)) + "." + std::to_string(XR_VERSION_MINOR(ver)) + "." + std::to_string(XR_VERSION_PATCH(ver));
}

void VRContext::Init(void *state){
    struct android_app* app = (struct android_app*)state;
    // Initialize the loader for this platform
    PFN_xrInitializeLoaderKHR initializeLoader = nullptr;
    if (XR_SUCCEEDED(
            xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction*)(&initializeLoader)))) {
        XrLoaderInitInfoAndroidKHR loaderInitInfoAndroid;
        memset(&loaderInitInfoAndroid, 0, sizeof(loaderInitInfoAndroid));
        loaderInitInfoAndroid.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
        loaderInitInfoAndroid.next = NULL;
        loaderInitInfoAndroid.applicationVM = app->activity->vm;
        loaderInitInfoAndroid.applicationContext = app->activity->clazz;
        initializeLoader((const XrLoaderInitInfoBaseHeaderKHR*)&loaderInitInfoAndroid);
        LOGW__("in:5");
    }
}

void VRContext::LogLayersAndExtensions() {
    const auto logExtensions = [](const char* layerName, int indent = 0) {
        uint32_t instanceExtensionCount;
        CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(layerName, 0, &instanceExtensionCount, nullptr));

        std::vector<XrExtensionProperties> extensions(instanceExtensionCount);
        for (XrExtensionProperties& extension : extensions) {
            extension.type = XR_TYPE_EXTENSION_PROPERTIES;
        }
        CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(layerName, (uint32_t)extensions.size(), &instanceExtensionCount,extensions.data()));
        const std::string indentStr(indent, ' ');
        LOGW__("%sAvailable Extensions: (%d)",indentStr.c_str(), instanceExtensionCount);
        for (const XrExtensionProperties& extension : extensions) {
            LOGW__("%s  Name=%s SpecVersion=%d",indentStr.c_str(), extension.extensionName,extension.extensionVersion);
        }
    };

    logExtensions(nullptr);

    // Log layers and any of their extensions.
    {
        uint32_t layerCount;
        CHECK_XRCMD(xrEnumerateApiLayerProperties(0, &layerCount, nullptr));

        std::vector<XrApiLayerProperties> layers(layerCount);
        for (XrApiLayerProperties& layer : layers) {
            layer.type = XR_TYPE_API_LAYER_PROPERTIES;
        }

        CHECK_XRCMD(xrEnumerateApiLayerProperties((uint32_t)layers.size(), &layerCount, layers.data()));
        LOGW__("Available Layers: (%d)", layerCount);
        for (const XrApiLayerProperties& layer : layers) {
            LOGW__("  Name=%s SpecVersion=%s LayerVersion=%d Description=%s", layer.layerName,GetXrVersionString(layer.specVersion).c_str(), layer.layerVersion, layer.description);
            logExtensions(layer.layerName, 4);
        }
    }
}

void VRContext::CreateInstanceInternal(void *state){
    struct android_app* data = (struct android_app*)state;
    std::vector<const char*> extensions;

    // Transform platform and graphics extension std::strings to C strings.
    const std::vector<std::string> platformExtensions = {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME};
    std::transform(platformExtensions.begin(), platformExtensions.end(), std::back_inserter(extensions),
                   [](const std::string& ext) { return ext.c_str(); });
    const std::vector<std::string> graphicsExtensions = {XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME};
    std::transform(graphicsExtensions.begin(), graphicsExtensions.end(), std::back_inserter(extensions),
                   [](const std::string& ext) { return ext.c_str(); });

    XrInstanceCreateInfoAndroidKHR instanceCreateInfoAndroid;
    instanceCreateInfoAndroid = {XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
    instanceCreateInfoAndroid.applicationVM =  data->activity->vm;
    instanceCreateInfoAndroid.applicationActivity = data->activity->clazz;
    XrInstanceCreateInfo createInfo{XR_TYPE_INSTANCE_CREATE_INFO};
    createInfo.next = (XrBaseInStructure*)&instanceCreateInfoAndroid;
    createInfo.enabledExtensionCount = (uint32_t)extensions.size();
    createInfo.enabledExtensionNames = extensions.data();

    strcpy(createInfo.applicationInfo.applicationName, "HelloXR");
    createInfo.applicationInfo.apiVersion = XR_CURRENT_API_VERSION;

    CHECK_XRCMD(xrCreateInstance(&createInfo, &m_instance));
    LOGW__("111222333444");
}

void VRContext::LogInstanceInfo() {
    XrInstanceProperties instanceProperties{XR_TYPE_INSTANCE_PROPERTIES};
    CHECK_XRCMD(xrGetInstanceProperties(m_instance, &instanceProperties));
    LOGI__("Instance RuntimeName=%s RuntimeVersion=%s", instanceProperties.runtimeName,GetXrVersionString(instanceProperties.runtimeVersion).c_str());

}

