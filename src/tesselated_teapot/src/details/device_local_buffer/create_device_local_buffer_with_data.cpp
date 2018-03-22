#include "create_device_local_buffer_with_data.h"

using namespace std;

namespace details
{
	MaybeCreateDeviceLocalBufferData create_device_local_buffer_with_data(CreateDeviceLocalBufferData bufferData)
	{
		MaybeCreateDeviceLocalBufferData mbCopyBufferData{create_staging_buffer(move(bufferData))
		                                                  .and_then(find_staging_buffer_memory_type)
		                                                  .and_then(create_staging_device_memory)
		                                                  .and_then(bind_staging_buffer_and_memory)
		                                                  .and_then(copy_data_to_staging_buffer)
		                                                  .and_then(create_device_local_buffer)
		                                                  .and_then(find_device_local_buffer_memory_type)
		                                                  .and_then(create_device_local_device_memory)
		                                                  .and_then(bind_device_local_buffer_and_memory)
		                                                  .and_then(create_copy_command_pool)
		                                                  .and_then(allocate_command_buffer)
		                                                  .and_then(copy_buffer)};
	
		return mbCopyBufferData;
	}
}