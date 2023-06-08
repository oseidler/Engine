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
	SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
	subscribers.push_back(functionPtr);
}


void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
	for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
	{
		if (subscribers[subIndex] == functionPtr)
		{
			subscribers[subIndex] = nullptr;
		}
	}
}


void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	auto foundEvent = m_subscriptionListByEventName.find(eventName);

	if (foundEvent != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
		for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
		{
			if (subscribers[subIndex] != nullptr)
			{
				bool isConsumed = subscribers[subIndex](args);

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
}


void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs args = EventArgs();

	auto foundEvent = m_subscriptionListByEventName.find(eventName);

	if (foundEvent != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
		for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
		{
			if (subscribers[subIndex] != nullptr)
			{
				bool isConsumed = subscribers[subIndex](args);

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
}


//
//accessors
//
std::vector<std::string> EventSystem::GetAllRegisteredEvents() const
{
	std::vector<std::string> listOfEvents;

	for (auto mapIterator = m_subscriptionListByEventName.begin(); mapIterator != m_subscriptionListByEventName.end(); mapIterator++)
	{
		listOfEvents.push_back(mapIterator->first);
	}

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
