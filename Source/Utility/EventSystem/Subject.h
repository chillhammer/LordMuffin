#pragma once
#include <skelpch.h>
#include "IObserver.h"

namespace Skel
{
	// Holds list of listeners to activate upon event. T is derived Event
	class Subject
	{
	protected:
		std::list<class IObserver*>	m_Observers;

	public:
		Subject() {};
		void AddObserver(class IObserver* const observer);
		void RemoveObserver(class IObserver* const observer);
		void Notify(class Event& event) const;
	};

}