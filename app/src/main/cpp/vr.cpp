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
#include <vector>

#include <android_native_app_glue.h>

#include <android/log.h>

#define LOGI__(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW__(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
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
//        uint32_t instanceExtensionCount;
//        CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(layerName, 0, &instanceExtensionCount, nullptr));
//
//        std::vector<XrExtensionProperties> extensions(instanceExtensionCount);
//        for (XrExtensionProperties& extension : extensions) {
//            extension.type = XR_TYPE_EXTENSION_PROPERTIES;
//        }
//
//        CHECK_XRCMD(xrEnumerateInstanceExtensionProperties(layerName, (uint32_t)extensions.size(), &instanceExtensionCount,
//                                                           extensions.data()));
//
//        const std::string indentStr(indent, ' ');
//        Log::Write(Log::Level::Verbose, Fmt("%sAvailable Extensions: (%d)", indentStr.c_str(), instanceExtensionCount));
//        for (const XrExtensionProperties& extension : extensions) {
//            Log::Write(Log::Level::Verbose, Fmt("%s  Name=%s SpecVersion=%d", indentStr.c_str(), extension.extensionName,
//                                                extension.extensionVersion));
//        }
    };
}

