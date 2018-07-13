#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_shader_modules(AppData data)
{
	assert(data.device);
	
	/*helpers::MaybeShaderModule const maybeVertexShaderModule{helpers::create_shader_module(appData.device, "VertexShader.vert.spv")};
	if(!maybeVertexShaderModule)
		return tl::make_unexpected(maybeVertexShaderModule.error());
		
	helpers::MaybeShaderModule const maybeTessControlShaderModule{helpers::create_shader_module(appData.device, "TesselationControlShader.tesc.spv")};
	if(!maybeTessControlShaderModule)
		return tl::make_unexpected(maybeTessControlShaderModule.error());
		
	helpers::MaybeShaderModule const maybeTessEvaluationShaderModule{helpers::create_shader_module(appData.device, "TesselationEvaluationShader.tese.spv")};
	if(!maybeTessEvaluationShaderModule)
		return tl::make_unexpected(maybeTessEvaluationShaderModule.error());
		
	helpers::MaybeShaderModule const maybeFragmentShaderModule{helpers::create_shader_module(appData.device, "FragmentShader.frag.spv")};
	if(!maybeFragmentShaderModule)
		return tl::make_unexpected(maybeFragmentShaderModule.error());
		
	appData.vertexShaderModule = *maybeVertexShaderModule;
	appData.tessControlShaderModule = *maybeTessControlShaderModule;
	appData.tessEvaluationShaderModule = *maybeTessEvaluationShaderModule;
	appData.fragmentShaderModule = *maybeFragmentShaderModule;*/
		
	return data;
}

} // namespace app
