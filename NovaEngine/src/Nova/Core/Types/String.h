#pragma once

#include <string>
#include <format>

namespace Nova
{
	// TODO: create our own string class for stable API

	/// <summary>
	/// Represents a string of characters
	/// </summary>
	using string = std::string;

	template <typename ... Args>
	string FormatString(const string& formatString, Args&& ... values)
	{
		return std::vformat(formatString, std::make_format_args(values...));
	}
}