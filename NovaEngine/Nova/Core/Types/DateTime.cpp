#include "DateTime.h"

#include <chrono>

namespace Nova
{
	TimeSpan::TimeSpan() : TimeSpan(0)
	{}

	TimeSpan::TimeSpan(uint64_t microseconds) :
		m_Microseconds(microseconds)
	{}

	TimeSpan TimeSpan::Now()
	{
		auto now = std::chrono::steady_clock::now();

		return TimeSpan(std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch().count());
	}

	const uint64_t TimeSpan::USecsPerMillisecond = 1000;
	const uint64_t TimeSpan::USecsPerSecond = 1000000;
	const uint64_t TimeSpan::USecsPerMinute = 60000000;
	const uint64_t TimeSpan::USecsPerHour = 3600000000;
	const uint64_t TimeSpan::USecsPerDay = 86400000000;

	uint64_t TimeSpan::GetMicroseconds() const
	{
		return m_Microseconds;
	}

	int TimeSpan::GetMilliseconds() const
	{
		return int((m_Microseconds % USecsPerSecond) / USecsPerMillisecond);
	}

	double TimeSpan::GetTotalMilliseconds() const
	{
		return m_Microseconds / (double)USecsPerMillisecond;
	}

	int TimeSpan::GetSeconds() const
	{
		return int((m_Microseconds % USecsPerMinute) / USecsPerSecond);
	}

	double TimeSpan::GetTotalSeconds() const
	{
		return m_Microseconds / (double)USecsPerSecond;
	}

	int TimeSpan::GetMinutes() const
	{
		return int((m_Microseconds % USecsPerHour) / USecsPerMinute);
	}

	double TimeSpan::GetTotalMinutes() const
	{
		return m_Microseconds / (double)USecsPerMinute;
	}

	int TimeSpan::GetHours() const
	{
		return int((m_Microseconds % USecsPerDay) / USecsPerHour);
	}

	double TimeSpan::GetTotalHours() const
	{
		return m_Microseconds / (double)USecsPerHour;
	}

	int TimeSpan::GetDays() const
	{
		return int(m_Microseconds / USecsPerDay);
	}

	double TimeSpan::GetTotalDays() const
	{
		return m_Microseconds / (double)USecsPerDay;
	}

	TimeSpan TimeSpan::operator+(const TimeSpan& rhs) const
	{
		return TimeSpan(m_Microseconds + rhs.m_Microseconds);
	}

	TimeSpan TimeSpan::operator-(const TimeSpan& rhs) const
	{
		return TimeSpan(m_Microseconds - rhs.m_Microseconds);
	}
}