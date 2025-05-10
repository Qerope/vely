#include <stdint.h>
#include <string.h>

/* Minimal subset of the Vulkan loader/ICD interface. No vulkan headers yet. */
typedef int32_t VkResult;
typedef void *VkInstance;
typedef void (*PFN_vkVoidFunction)(void);

#define VK_SUCCESS 0
#define VK_ERROR_INCOMPATIBLE_DRIVER (-9)

VkResult vk_icdNegotiateLoaderICDInterfaceVersion(uint32_t *version)
{
    /* version 5 is the first that has vk_icdGetInstanceProcAddr */
    if (*version < 5) return VK_ERROR_INCOMPATIBLE_DRIVER;
    *version = 5;
    return VK_SUCCESS;
}

PFN_vkVoidFunction vk_icdGetInstanceProcAddr(VkInstance inst, const char *name)
{
    (void)inst;
    return NULL;  /* everything else is unimplemented */
}
