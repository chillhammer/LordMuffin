#pragma once
#include <skelpch.h>
#include <EventSystem/IObserver.h>
#include "EventType.h"
namespace Skel
{
	// An event is the data passed from Subject to Observer
	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		bool Handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	///////////////////////////////////////////////
	// Macros for creating/handling events       //
	///////////////////////////////////////////////
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() \
										{ return EventType::##type; }\
									virtual EventType GetEventType() const override \
										{ return GetStaticType(); }\
									virtual const char* GetName() const override \
										{ return #type; }

	// An event
	#define EVENT_CLASS_SIMPLE(type, properties) \
		class type ## Event : public Event \
		{ \
		public: \
			EVENT_CLASS_TYPE(type) \
			properties \
		}

	// A signal class creator. Only difference currently is the name
	#define SIGNAL_CLASS(type, properties) \
		class name ## Signal : public Event \
		{ \
		public: \
			EVENT_CLASS_TYPE(type) \
			properties \
		}
	// Allows binding functions with 1 argument. Usually used with Dispatch
	// Example Usage: Dispatch<Event1>(event, EVENT_BIND_FN(Window, OnEvent1));
	#define EVENT_BIND_FN(Class, function) std::bind(&Skel::##Class##::function, this, std::placeholders::_1)
	////////////////////////////////////////////////
	// Namespace for functions dealing with Event //
	////////////////////////////////////////////////
	namespace Evnt
	{
		// Dispatches given event to class method
		template<typename T>
		bool Dispatch(Event& event, std::function<bool(T&)> func)
		{
			if (event.GetEventType() == T::GetStaticType())
			{
				event.Handled = func(*(T*)&event);
				return true;
			}
			return false;
		}
	}
}