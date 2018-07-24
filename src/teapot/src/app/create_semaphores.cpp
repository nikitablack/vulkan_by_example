#include "app/helpers/VkObjectsHelpers.h"
#include "app/App.h"

#include <cassert>

namespace app
{

MaybeAppData create_semaphores(AppData data)
{
	assert(data.device);
	
	{
		helpers::MaybeSemaphore const mbSemaphore{helpers::create_semaphore(data.device)};
		if (!mbSemaphore)
			return tl::make_unexpected(mbSemaphore.error());
		
		data.presentFinishedSemaphore = *mbSemaphore;
		
		helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_SEMAPHORE, reinterpret_cast<uint64_t>(data.presentFinishedSemaphore), "present finished semaphore");
	}
	
	{
		helpers::MaybeSemaphore const mbSemaphore{helpers::create_semaphore(data.device)};
		if (!mbSemaphore)
			return tl::make_unexpected(mbSemaphore.error());
		
		data.imageAvailableSemaphore = *mbSemaphore;
		
		helpers::set_debug_utils_object_name(data.instance, data.device, VK_OBJECT_TYPE_SEMAPHORE, reinterpret_cast<uint64_t>(data.imageAvailableSemaphore), "image available semaphore");
	}
	
	return data;
}

} // namespace app