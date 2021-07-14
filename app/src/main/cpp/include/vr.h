#pragma once
#ifndef _VR_H_
#define _VR_H_

class VRContext{
public:
    VRContext(){}
    ~VRContext(){}
    VRContext(const VRContext&) = delete;
    VRContext& operator=(const VRContext&) = delete;
    VRContext(VRContext&&) noexcept = default;
    VRContext& operator=(VRContext&&) noexcept = default;

    void Init(void *app);
    void LogLayersAndExtensions();
private:
};
#endif