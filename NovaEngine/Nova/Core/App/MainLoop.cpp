#include "MainLoop.h"

#include "App.h"

#include <iostream>
#include <format>

namespace Nova
{
	void MainLoop::Run()
	{
		m_IsRunning = true;

		while (m_IsRunning)
		{
			string a;
			std::cin >> a;

			App::Log(std::format("Input \"{0}\"", a));

			if (a.length() == 1)
			{
				Stop();
			}
		}
	}

	void MainLoop::Stop()
	{
		m_IsRunning = false;
	}
}