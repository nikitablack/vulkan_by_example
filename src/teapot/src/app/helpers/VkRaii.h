#pragma once

#include "vulkan/vulkan.hpp"

#include <functional>

namespace app::helpers
{

template<typename T>
class VkRaii
{
public:
	VkRaii() : m_obj{VK_NULL_HANDLE}, m_deleter{[](T){}} {}
	explicit VkRaii(std::function<void(T)> deleter) : m_obj{VK_NULL_HANDLE}, m_deleter{deleter} {}
	VkRaii(T obj, std::function<void(T)> deleter) : m_obj(obj), m_deleter(deleter) {}
	
	~VkRaii()
	{
		if (m_obj)
			m_deleter(m_obj);
	}
	
	T releaseAndSetNew(T obj)
	{
		T tmp{release()};
		
		m_obj = obj;
		
		return tmp;
	}
	
	T get() const
	{
		return m_obj;
	}
	
	T release()
	{
		T tmp{m_obj};
		m_obj = VK_NULL_HANDLE;
		return tmp;
	}
	
	VkRaii(VkRaii && other) noexcept : m_obj(other.m_obj), m_deleter(move(other.m_deleter))
	{
		other.m_obj = VK_NULL_HANDLE;
	}
	
	VkRaii & operator=(VkRaii && other) noexcept
	{
		m_obj = other.m_obj;
		m_deleter = move(other.m_deleter);
		other.m_obj = VK_NULL_HANDLE;
		return *this;
	}
	
	VkRaii & operator=(VkRaii const &) = delete;
	
	VkRaii(VkRaii const &) = delete;

private:
	T m_obj;
	std::function<void(T &)> m_deleter;
};

inline auto get_device_deleter()
{
	return [](VkDevice const device) { vkDestroyDevice(device, nullptr); };
}

inline auto get_buffer_deleter(VkDevice const device)
{
	return [device](VkBuffer const buffer) { if (device) vkDestroyBuffer(device, buffer, nullptr); };
}

inline auto get_device_memory_deleter(VkDevice const device)
{
	return [device](VkDeviceMemory const deviceMemory) { if (device) vkFreeMemory(device, deviceMemory, nullptr); };
}

inline auto get_command_pool_deleter(VkDevice const device)
{
	return [device](VkCommandPool const commandPool) { if (device) vkDestroyCommandPool(device, commandPool, nullptr); };
}

inline auto get_image_deleter(VkDevice const device)
{
	return [device](VkImage const image) { if (device) vkDestroyImage(device, image, nullptr); };
}

inline auto get_image_view_deleter(VkDevice const device)
{
	return [device](VkImageView const imageView) { if (device) vkDestroyImageView(device, imageView, nullptr); };
}

inline auto get_framebuffer_deleter(VkDevice const device)
{
	return [device](VkFramebuffer const framebuffer) { if (device) vkDestroyFramebuffer(device, framebuffer, nullptr); };
}

} // namespace app::helpers