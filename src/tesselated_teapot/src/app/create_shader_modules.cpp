#include "app/App.h"
#include "vk_helpers/VkObjectsHelper.h"

#include <cassert>

using namespace std;
using namespace tl;
using namespace vk_helpers;

namespace app
{
	MaybeAppData create_shader_modules(AppData appData)
	{
		assert(appData.device);
	
		MaybeShaderModule const maybeVertexShaderModule{create_shader_module(appData.device, "VertexShader.spv")};
		if(!maybeVertexShaderModule)
			return make_unexpected(maybeVertexShaderModule.error());
		
		MaybeShaderModule const maybeTessControlShaderModule{create_shader_module(appData.device, "TesselationControlShader.spv")};
		if(!maybeTessControlShaderModule)
			return make_unexpected(maybeTessControlShaderModule.error());
		
		MaybeShaderModule const maybeTessEvaluationShaderModule{create_shader_module(appData.device, "TesselationEvaluationShader.spv")};
		if(!maybeTessEvaluationShaderModule)
			return make_unexpected(maybeTessEvaluationShaderModule.error());
		
		MaybeShaderModule const maybeFragmentShaderModule{create_shader_module(appData.device, "FragmentShader.spv")};
		if(!maybeFragmentShaderModule)
			return make_unexpected(maybeFragmentShaderModule.error());
		
		appData.vertexShaderModule = *maybeVertexShaderModule;
		appData.tessControlShaderModule = *maybeTessControlShaderModule;
		appData.tessEvaluationShaderModule = *maybeTessEvaluationShaderModule;
		appData.fragmentShaderModule = *maybeFragmentShaderModule;
		
		return appData;
	}
}
