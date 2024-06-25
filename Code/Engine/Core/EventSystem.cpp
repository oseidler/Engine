#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"


//global event system pointer
EventSystem* g_theEventSystem = nullptr;


//constructor
EventSystem::EventSystem(EventSystemConfig const& config)
	: m_config(config)
{
}


//
//event functions
//
void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	m_subscriptionListMutex.lock();
	SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
	subscribers.push_back(new EventSubscriptionFunction(functionPtr));
	m_subscriptionListMutex.unlock();
}


void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	m_subscriptionListMutex.lock();
	
	auto foundEvent = m_subscriptionListByEventName.find(eventName);
	if (foundEvent == m_subscriptionListByEventName.end())
	{
		m_subscriptionListMutex.unlock();
		return;
	}

	SubscriptionList& subscribers = foundEvent->second;
	for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
	{
		EventSubscriptionBase*& subscriber = subscribers[subIndex];
		EventSubscriptionFunction* subFunc = dynamic_cast<EventSubscriptionFunction*>(subscriber);
		if (subFunc != nullptr && subFunc->m_funcPtr == functionPtr)
		{
			delete subFunc;
			subscriber = nullptr;
		}
	}

	m_subscriptionListMutex.unlock();
}


void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	m_subscriptionListMutex.lock();
	auto foundEvent = m_subscriptionListByEventName.find(eventName);

	if (foundEvent != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscribers = foundEvent->second;
		for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
		{
			EventSubscriptionBase*& subscriber = subscribers[subIndex];
			if (subscriber != nullptr)
			{
				m_subscriptionListMutex.unlock();
				
				bool isConsumed = subscriber->Execute(args);

				m_subscriptionListMutex.lock();

				std::string errorString = args.GetValue("error", "");
				if (errorString != "")
				{
					if (g_theDevConsole != nullptr)
					{
						g_theDevConsole->AddLine(DevConsole::COLOR_ERROR, errorString);
						std::string exampleString = args.GetValue("example", "");
						if (exampleString != "")
						{
							g_theDevConsole->AddLine(DevConsole::COLOR_WARNING, exampleString);
						}
					}
				}

				if (isConsumed)
				{
					m_subscriptionListMutex.unlock();
					return;
				}
			}
		}
	}
	else
	{
		if (g_theDevConsole != nullptr)
		{
			std::string errorString = "Unknown command/event: " + eventName;
			g_theDevConsole->AddLine(DevConsole::COLOR_ERROR, errorString);
		}
	}
	m_subscriptionListMutex.unlock();
}


void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs args = EventArgs();
	FireEvent(eventName, args);
}


//
//accessors
//
std::vector<std::string> EventSystem::GetAllRegisteredEvents()
{
	std::vector<std::string> listOfEvents;

	m_subscriptionListMutex.lock();
	for (auto mapIterator = m_subscriptionListByEventName.begin(); mapIterator != m_subscriptionListByEventName.end(); mapIterator++)
	{
		listOfEvents.push_back(mapIterator->first.GetOriginalText());
	}
	m_subscriptionListMutex.unlock();

	return listOfEvents;
}


//
//standalone global event functions
//
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
}


void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
}


void FireEvent(std::string const& eventName, EventArgs& args)
{
	g_theEventSystem->FireEvent(eventName, args);
}


void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(eventName);
}
