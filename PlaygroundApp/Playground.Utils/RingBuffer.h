
#pragma once

#include <condition_variable>
#include <mutex>
#include <span>
#include <vector>

namespace Playground::Utils
{
	struct RingBuffer
	{
		RingBuffer(size_t size)
		{
			m_buffer = std::vector<char>(size);
			m_head = 0;
			m_tail = 0;
			m_count = 0;
		}

		void Enqueue(std::span<const char> message)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_empty.wait(lock, [this]() -> bool
				{
					return m_count == 0;
				});

			for (int i = 0; i < message.size(); ++i)
			{
				m_buffer[m_head] = message[i];
				m_head++;
			}

			m_count += message.size();

			m_full.notify_all();
		}

		std::span<char> Dequeue()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_full.wait(lock, [this]() -> bool
				{
					return m_count > 0;
				});

			auto result = std::span<char>(m_buffer);
			m_buffer.clear();

			m_empty.notify_all();

			return result;
		}

	private:
		std::mutex m_mutex;
		std::condition_variable m_empty;
		std::condition_variable m_full;
		size_t m_head, m_tail, m_count;
		std::vector<char> m_buffer;
	};
}