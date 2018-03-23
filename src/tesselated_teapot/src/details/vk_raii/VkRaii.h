#pragma once

#include <vulkan/vulkan.h>
#include <functional>

namespace details
{
	template<typename T>
	class VkRaii
	{
	public:
		VkRaii() : m_obj(VK_NULL_HANDLE), m_deleter([](T) {}) {}
		
		VkRaii(T obj, std::function<void(T)> deleter) : m_obj(obj), m_deleter(deleter) {}
		
		~VkRaii()
		{
			if (m_obj)
				m_deleter(m_obj);
		}
		
		T get()
		{
			return m_obj;
		}
		
		const T get() const
		{
			return m_obj;
		}
		
		T release()
		{
			T tmp{m_obj};
			m_obj = VK_NULL_HANDLE;
			return tmp;
		}
		
		VkRaii(VkRaii &&other) noexcept : m_obj(other.m_obj), m_deleter(move(other.m_deleter))
		{
			other.m_obj = VK_NULL_HANDLE;
		}
		
		VkRaii &operator=(VkRaii &&other) noexcept
		{
			m_obj = other.m_obj;
			m_deleter = move(other.m_deleter);
			other.m_obj = VK_NULL_HANDLE;
			return *this;
		}
		
		VkRaii &operator=(VkRaii const &) = delete;
		
		VkRaii(VkRaii const &) = delete;
	
	private:
		T m_obj;
		std::function<void(T &)> m_deleter;
	};
	
	inline auto getBufferDeleter(VkDevice const device)
	{
		return [device](VkBuffer buffer) {if (device) vkDestroyBuffer(device, buffer, nullptr); };
	}
	
	inline auto getDeviceMemoryDeleter(VkDevice const device)
	{
		return [device](VkDeviceMemory deviceMemory) {if (device) vkFreeMemory(device, deviceMemory, nullptr); };
	}
	
	inline auto getCommandPoolDeleter(VkDevice const device)
	{
		return [device](VkCommandPool commandPool) {if (device) vkDestroyCommandPool(device, commandPool, nullptr); };
	}
}