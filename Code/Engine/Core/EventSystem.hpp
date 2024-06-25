#pragma once
#include "Engine/Core/NamedProperties.hpp"
#include <vector>
#include <mutex>


typedef NamedProperties EventArgs;
typedef bool (*EventCallbackFunction) (EventArgs& args);


class EventSubscriptionBase
{
	friend class EventSystem;

protected:
	virtual ~EventSubscriptionBase() = default;
	virtual bool Execute(EventArgs& args) = 0;
	virtual bool IsMethodOfQueriedObject(void* queriedObject) = 0;
};


//class for standalone/static functions
class EventSubscriptionFunction : public EventSubscriptionBase
{
	friend class EventSystem;

	EventSubscriptionFunction(EventCallbackFunction funcPtr)
		:m_funcPtr(funcPtr)
	{}

	virtual bool Execute(EventArgs& args) override
	{
		return m_funcPtr(args);
	}

	virtual bool IsMethodOfQueriedObject(void* queriedObject) override 
	{
		(void)(queriedObject);	//prevent unused warning
		return false;
	}

	EventCallbackFunction m_funcPtr;
};


//template for methods on objects
template<typename T>
class EventSubscriptionObjectMethod : public EventSubscriptionBase
{
	typedef bool (T::* EventCallbackObjectMethod)(EventArgs& args);

	friend class EventSystem;

	EventSubscriptionObjectMethod(T& objectInstance, EventCallbackObjectMethod methodPtr)
		: m_objectInstance(objectInstance)
		, m_methodPtr(methodPtr)
	{}

	virtual bool Execute(EventArgs& args) override
	{
		return (m_objectInstance.*m_methodPtr)(args);
	}

	virtual bool IsMethodOfQueriedObject(void* queriedObject)
	{
		return queriedObject == &m_objectInstance;
	}

	T& m_objectInstance;
	EventCallbackObjectMethod m_methodPtr;
};


typedef std::vector<EventSubscriptionBase*> SubscriptionList;


//config struct
struct EventSystemConfig
{
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

	//event function templates
	template<typename T>
	void SubscribeEventCallbackObjectMethod(std::string const& eventName, T& objectInstance, bool (T::* objectMethod)(EventArgs& args));
	//unsubscribe specific method for specific object
	template<typename T>
	void UnsubscribeEventCallbackObjectMethod(std::string const& eventName, T& objectInstance, bool (T::* objectMethod)(EventArgs& args));
	//unsubscribe all methods for specific object
	template<typename T>
	void UnsubscribeEventCallbackObject(T& objectInstance);

	//accessors
	std::vector<std::string> GetAllRegisteredEvents();

//private member variables
private:
	EventSystemConfig						m_config;
	std::map<HCIString, SubscriptionList>	m_subscriptionListByEventName;
	std::mutex								m_subscriptionListMutex;
};


//template function definitions
template<typename T>
void EventSystem::SubscribeEventCallbackObjectMethod(std::string const& eventName, T& objectInstance, bool (T::* objectMethod)(EventArgs& args))
{
	m_subscriptionListMutex.lock();
	SubscriptionList& subscribers = m_subscriptionListByEventName[eventName];
	subscribers.push_back(new EventSubscriptionObjectMethod<T>(objectInstance, objectMethod));
	m_subscriptionListMutex.unlock();
}


template<typename T>
void EventSystem::UnsubscribeEventCallbackObjectMethod(std::string const& eventName, T& objectInstance, bool (T::* objectMethod)(EventArgs& args))
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
		EventSubscriptionObjectMethod<T>* subMethod = dynamic_cast<EventSubscriptionObjectMethod<T>*>(subscriber);
		if (subMethod != nullptr && &subMethod->m_objectInstance == &objectInstance && subMethod->m_methodPtr == objectMethod)
		{
			delete subMethod;
			subscriber = nullptr;
		}
	}

	m_subscriptionListMutex.unlock();
}


template<typename T>
void EventSystem::UnsubscribeEventCallbackObject(T& objectInstance)
{
	m_subscriptionListMutex.lock();
	
	//search entire map for subscriptions of correct type
	std::map<HCIString, SubscriptionList>::iterator iter;
	for (iter = m_subscriptionListByEventName.begin(); iter != m_subscriptionListByEventName.end(); iter++)
	{
		SubscriptionList& subscribers = iter->second;

		//check each subscriber to this event
		for (int subIndex = 0; subIndex < subscribers.size(); subIndex++)
		{
			EventSubscriptionBase*& subscriber = subscribers[subIndex];
			//EventSubscriptionObjectMethod<T>* subMethod = dynamic_cast<EventSubscriptionObjectMethod<T>*>(subscriber);

			////if subscription is of correct type and subscriber is the correct specific object, delete its subscription for this method
			//if (subMethod != nullptr && &subMethod->m_objectInstance == &objectInstance)
			//{
			//	delete subMethod;
			//	subscriber = nullptr;
			//}

			if (subscriber != nullptr && subscriber->IsMethodOfQueriedObject(&objectInstance))
			{
				delete subscriber;
				subscriber = nullptr;
			}
		}
	}
	
	m_subscriptionListMutex.unlock();
}


//standalone global functions
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);


//external variable declaration
extern EventSystem* g_theEventSystem;


//event recipient base class
class EventRecipient
{
protected:
	virtual ~EventRecipient()
	{
		g_theEventSystem->UnsubscribeEventCallbackObject(*this);
	}
};
