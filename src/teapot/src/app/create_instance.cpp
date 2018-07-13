#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

namespace app
{

MaybeAppData create_instance(AppData appData)
{
	helpers::MaybeInstance const mbInstance{helpers::create_instance(&appData.instanceExtensions, &appData.layers)};
	if(!mbInstance)
		return tl::make_unexpected(mbInstance.error());
	
	appData.instance = *mbInstance;
	
	return appData;
}

}