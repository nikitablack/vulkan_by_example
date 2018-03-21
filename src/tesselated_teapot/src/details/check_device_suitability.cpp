#include "details/Details.h"

using namespace std;

namespace details
{
	bool check_device_suitability(VkPhysicalDevice const physicalDevice, vector<char const *> const & requiredExtensions)
	{
		VkPhysicalDeviceProperties deviceProperties{};
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		
		if (deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			return false;
		
		VkPhysicalDeviceFeatures deviceFeatures{};
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		
		if (!deviceFeatures.tessellationShader)
			return false;
		
		if (deviceProperties.limits.maxTessellationPatchSize < 16)
			return false;
		
		if (!deviceFeatures.fillModeNonSolid)
			return false;
		
		if (!check_required_device_extensions(physicalDevice, requiredExtensions))
			return false;
		
		return true;
	}
}