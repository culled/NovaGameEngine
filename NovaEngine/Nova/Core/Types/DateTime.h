#pragma once

#include "Nova/Core/Engine.h"

#include "String.h"

namespace Nova
{
	/// <summary>
	/// Represents a duration of time with microsecond precision
	/// </summary>
	NovaStruct TimeSpan
	{
	public:
		/// <summary>
		/// Creates a TimeSpan representing a zero duration
		/// </summary>
		TimeSpan();

		/// <summary>
		/// Creates a TimeSpan with the given duration of microseconds
		/// </summary>
		/// <param name="microseconds">The number of microseconds that have passed</param>
		TimeSpan(uint64_t microseconds);

	public:
		/// <summary>
		/// Gets the duration of time since epoch
		/// </summary>
		/// <returns>A TimeSpan that represents the duration of time since epoch</returns>
		static TimeSpan Now();

	public:
		// The amount of microseconds in 1 millisecond
		static const uint64_t USecsPerMillisecond;

		// The amount of microseconds in 1 second
		static const uint64_t USecsPerSecond;

		// The amount of microseconds in 1 minute
		static const uint64_t USecsPerMinute;

		// The amount of microseconds in 1 hour
		static const uint64_t USecsPerHour;

		// The amount of microseconds in 1 day
		static const uint64_t USecsPerDay;

	public:
		/// <summary>
		/// Gets the number of microseconds of this TimeSpan
		/// </summary>
		/// <returns>The number of microseconds of this TimeSpan</returns>
		uint64_t GetMicroseconds() const;

		/// <summary>
		/// Gets the milliseconds component of this TimeSpan
		/// </summary>
		/// <returns>The milliseconds component of this TimeSpan</returns>
		int GetMilliseconds() const;

		/// <summary>
		/// Gets the total number of milliseconds that this TimeSpan's duration contains
		/// </summary>
		/// <returns>The total number of milliseconds in this TimeSpan's duration</returns>
		double GetTotalMilliseconds() const;

		/// <summary>
		/// Gets the seconds component of this TimeSpan
		/// </summary>
		/// <returns>The seconds component of this TimeSpan</returns>
		int GetSeconds() const;

		/// <summary>
		/// Gets the total number of seconds that this TimeSpan's duration contains
		/// </summary>
		/// <returns>The total number of seconds in this TimeSpan's duration</returns>
		double GetTotalSeconds() const;

		/// <summary>
		/// Gets the minutes component of this TimeSpan
		/// </summary>
		/// <returns>The minutes component of this TimeSpan</returns>
		int GetMinutes() const;

		/// <summary>
		/// Gets the total number of minutes that this TimeSpan's duration contains
		/// </summary>
		/// <returns>The total number of minutes in this TimeSpan's duration</returns>
		double GetTotalMinutes() const;

		/// <summary>
		/// Gets the hours component of this TimeSpan
		/// </summary>
		/// <returns>The hours component of this TimeSpan</returns>
		int GetHours() const;

		/// <summary>
		/// Gets the total number of hours that this TimeSpan's duration contains
		/// </summary>
		/// <returns>The total number of hours in this TimeSpan's duration</returns>
		double GetTotalHours() const;

		/// <summary>
		/// Gets the days component of this TimeSpan
		/// </summary>
		/// <returns>The days component of this TimeSpan</returns>
		int GetDays() const;

		/// <summary>
		/// Gets the total number of days that this TimeSpan's duration contains
		/// </summary>
		/// <returns>The total number of days in this TimeSpan's duration</returns>
		double GetTotalDays() const;

		TimeSpan operator+(const TimeSpan& rhs) const;
		TimeSpan operator-(const TimeSpan& rhs) const;

	private:
		// The number of microseconds in this TimeSpan
		uint64_t m_Microseconds;
	};
}