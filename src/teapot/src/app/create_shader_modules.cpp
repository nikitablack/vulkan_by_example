#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>
#include <fstream>

using namespace std;

namespace
{
	using MaybeShaderData = tl::expected<vector<char>, string>;

	MaybeShaderData load_shader(string const & fileName)
	{
		ifstream file{fileName, ios::ate | ios::binary};

		if (!file.is_open())
			return tl::make_unexpected("failed to open shader file");

		size_t const fileSize{static_cast<size_t>(file.tellg())};
		vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		if (!file)
			return tl::make_unexpected("failed to read shader file");

		if (buffer.empty() || buffer.size() % 4 != 0)
			return tl::make_unexpected("failed to read shader file");

		return buffer;
	}
}

namespace app
{

MaybeAppData create_shader_modules(AppData data)
{
	assert(data.device);
	
	{
		MaybeShaderData const mbShaderData{load_shader("VertexShader.spv")};
		if (!mbShaderData)
			tl::make_unexpected(mbShaderData.error());

		helpers::MaybeShaderModule const mbVertexShaderModule{helpers::create_shader_module(data.device, &(*mbShaderData))};
		if (!mbVertexShaderModule)
			return tl::make_unexpected(mbVertexShaderModule.error());

		data.vertexShaderModule = *mbVertexShaderModule;
	}

	{
		MaybeShaderData const mbShaderData{load_shader("TesselationControlShader.spv")};
		if (!mbShaderData)
			tl::make_unexpected(mbShaderData.error());

		helpers::MaybeShaderModule const mbTessControlShaderModule{helpers::create_shader_module(data.device, &(*mbShaderData))};
		if (!mbTessControlShaderModule)
			return tl::make_unexpected(mbTessControlShaderModule.error());

		data.tessControlShaderModule = *mbTessControlShaderModule;
	}

	{
		MaybeShaderData const mbShaderData{load_shader("TesselationEvaluationShader.spv")};
		if (!mbShaderData)
			tl::make_unexpected(mbShaderData.error());

		helpers::MaybeShaderModule const mbTessEvaluationShaderModule{helpers::create_shader_module(data.device, &(*mbShaderData))};
		if (!mbTessEvaluationShaderModule)
			return tl::make_unexpected(mbTessEvaluationShaderModule.error());

		data.tessEvaluationShaderModule = *mbTessEvaluationShaderModule;
	}

	{
		MaybeShaderData const mbShaderData{load_shader("FragmentShader.spv")};
		if (!mbShaderData)
			tl::make_unexpected(mbShaderData.error());

		helpers::MaybeShaderModule const mbFragmentShaderModule{helpers::create_shader_module(data.device, &(*mbShaderData))};
		if (!mbFragmentShaderModule)
			return tl::make_unexpected(mbFragmentShaderModule.error());

		data.fragmentShaderModule = *mbFragmentShaderModule;
	}

	return data;
}

} // namespace app
