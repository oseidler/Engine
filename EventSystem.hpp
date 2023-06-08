#pragma once
#include <vector>
#include <string>
#include <map>
#include "Engine/Core/NamedStrings.hpp"


//typedefs
typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction) (EventArgs& args);
typedef std::vector<EventCallbackFunction> SubscriptionList;


//config struct
struct EventSystemConfig
{
	//empty for now
};


class EventSystem
{
//public member functions
public:
	//constructor and destructor
	EventSystem(EventSystemConfig const& config);
	~EventSystem() {}

	//game flow functions
	void Startup() {}
	void Shutdown() {}
	void BeginFrame() {}
	void EndFrame() {}

	//event functions
	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);

	//accessors
	std::vector<std::string> GetAllRegisteredEvents() const;

//private member variables
private:
	EventSystemConfig						m_config;
	std::map<std::string, SubscriptionList>	m_subscriptionListByEventName;
};

//standalone global functions
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);

//external variable declaration
extern EventSystem* g_theEventSystem;
