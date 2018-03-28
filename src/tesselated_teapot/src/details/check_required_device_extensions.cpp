#include "details/Details.h"

#include <algorithm>
#include <cstring>

using namespace std;

namespace details
{
	bool check_required_device_extensions(VkPhysicalDevice const physicalDevice, vector<char const *> const & requiredExtensions)
	{
		uint32_t extensionCount{0};
		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr) != VK_SUCCESS)
			return false;
		
		vector<VkExtensionProperties> availableExtensions(extensionCount);
		if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data()) != VK_SUCCESS)
			return false;
		
		for (char const * element : requiredExtensions)
		{
			if (find_if(availableExtensions.begin(), availableExtensions.end(), [element](VkExtensionProperties const & extensionProp) { return strcmp(element, extensionProp.extensionName) == 0; }) == availableExtensions.end())
				return false;
		}
		
		return true;
	}
}