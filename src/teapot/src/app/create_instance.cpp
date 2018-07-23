#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

namespace app
{

MaybeAppData create_instance(AppData data)
{
	helpers::MaybeInstance const mbInstance{helpers::create_instance(&data.instanceExtensions, &data.layers)};
	if(!mbInstance)
		return tl::make_unexpected(mbInstance.error());
	
	data.instance = *mbInstance;
	
	return data;
}

} // namespace app