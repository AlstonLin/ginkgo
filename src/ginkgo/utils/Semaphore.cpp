#include <ginkgo/utils/Semaphore.h>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Semaphore
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Semaphore::Semaphore()
:	m_count{0},
	m_isActive{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Semaphore::notify()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_count++;
	m_condition.notify_one();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Semaphore::notifyFinished()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isActive = false;
	m_condition.notify_one();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Semaphore::wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	m_condition.wait(lock,
		[&]()
		{
			return m_count > 0 || !m_isActive;
		});

	if (m_count > 0)
		m_count--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::cv_status Semaphore::waitFor(const std::chrono::milliseconds &relativeTime)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	const auto result = m_condition.wait_for(lock, relativeTime,
		[&]()
		{
			return m_count > 0 || !m_isActive;
		});

	if (!result)
		return std::cv_status::timeout;

	if (m_count > 0)
		m_count--;

	return std::cv_status::no_timeout;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t Semaphore::count() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Semaphore::isActive() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_isActive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Semaphore::reset()
{
	// TODO: Don't make reset calls mandatory
	m_count = 0;
	m_isActive = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
