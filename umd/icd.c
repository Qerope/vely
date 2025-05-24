#include <stdint.h>
#include <string.h>

/* Minimal subset of the Vulkan loader/ICD interface. No vulkan headers yet. */
typedef int32_t VkResult;
typedef void *VkInstance;
typedef void (*PFN_vkVoidFunction)(void);

#define VK_SUCCESS 0
#define VK_ERROR_INCOMPATIBLE_DRIVER (-9)
#define VK_INCOMPLETE 5

VkResult vk_icdNegotiateLoaderICDInterfaceVersion(uint32_t *version)
{
    /* version 5 is the first that has vk_icdGetInstanceProcAddr */
    if (*version < 5) return VK_ERROR_INCOMPATIBLE_DRIVER;
    *version = 5;
    return VK_SUCCESS;
}

VkResult vely_EnumeratePhysicalDevices(VkInstance inst, uint32_t *count, void **devices)
{
    (void)inst;
    static int dummy_device;
    if (!devices) { *count = 1; return VK_SUCCESS; }
    if (*count < 1) return VK_INCOMPLETE;
    devices[0] = &dummy_device;
    *count = 1;
    return VK_SUCCESS;
}

PFN_vkVoidFunction vk_icdGetInstanceProcAddr(VkInstance inst, const char *name)
{
    (void)inst;
    if (!strcmp(name, "vkEnumeratePhysicalDevices")) return (PFN_vkVoidFunction)vely_EnumeratePhysicalDevices;
    return NULL;  /* everything else is unimplemented */
}
