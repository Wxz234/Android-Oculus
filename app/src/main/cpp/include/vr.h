#pragma once
#ifndef _VR_H_
#define _VR_H_
#include <openxr/openxr.h>
class VRContext{
public:
    VRContext() :m_instance{XR_NULL_HANDLE}{ }
    ~VRContext(){}
    VRContext(const VRContext&) = delete;
    VRContext& operator=(const VRContext&) = delete;
    VRContext(VRContext&&) noexcept = default;
    VRContext& operator=(VRContext&&) noexcept = default;

    void Init(void *app);
    void LogLayersAndExtensions();
    void CreateInstanceInternal(void *state);
    void LogInstanceInfo();
private:
    XrInstance m_instance;
};
#endif