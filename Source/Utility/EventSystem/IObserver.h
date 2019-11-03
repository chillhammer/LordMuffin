#pragma once

namespace Skel
{
	// Interface to recieve events
	class IObserver
	{
		friend class Subject;
	public:

		virtual void OnEvent(const Subject* subject, class Event& event) = 0;


	};
}