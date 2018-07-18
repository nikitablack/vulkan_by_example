#include "app/App.h"

namespace app
{

AppData prepare_device_features(AppData data)
{
	data.physicalDeviceFeatures.tessellationShader = VK_TRUE;
	data.physicalDeviceFeatures.fillModeNonSolid = VK_TRUE;
	data.physicalDeviceFeatures.vertexPipelineStoresAndAtomics = VK_TRUE; // // though the patch buffer in the tesselation evaluation shader marked as `readonly` the validation layer still complains ("Shader requires vertexPipelineStoresAndAtomics but is not enabled on the device"). This is a bug - https://github.com/KhronosGroup/Vulkan-ValidationLayers/issues/73.
	
	return data;
}

} // namespace app