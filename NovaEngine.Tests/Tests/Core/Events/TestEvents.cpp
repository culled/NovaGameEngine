#include <catch.hpp>

#include "EventObj.h"
#include <Nova/Core/Events/EventSource.h>

TEST_CASE("Nova/Core/Events/Ref Event Listener", "Test connecting and disconnecting a Ref to an event source")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> obj = std::make_shared<EventObj>();

	eventSource.Connect(obj, &EventObj::Callback);

	REQUIRE(obj->EventsReceived == 0);

	eventSource.EmitAnonymous();

	REQUIRE(obj->EventsReceived == 1);

	eventSource.Disconnect(obj, &EventObj::Callback);

	eventSource.EmitAnonymous();

	REQUIRE(obj->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Ptr Event Listener", "Test connecting and disconnecting a pointer to an event source")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Exclusive<EventObj> obj = std::make_unique<EventObj>();

	eventSource.Connect(obj.get(), &EventObj::Callback);

	REQUIRE(obj->EventsReceived == 0);

	eventSource.EmitAnonymous();

	REQUIRE(obj->EventsReceived == 1);

	eventSource.Disconnect(obj.get(), &EventObj::Callback);

	eventSource.EmitAnonymous();

	REQUIRE(obj->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Disconnect Event", "Test if an event listener disconnects properly")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> objA = std::make_shared<EventObj>();
	Nova::Ref<EventObj> objB = std::make_shared<EventObj>();

	eventSource.Connect(objA, &EventObj::Callback);
	eventSource.Connect(objB, &EventObj::Callback);

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 0);

	eventSource.EmitAnonymous();

	REQUIRE(objA->EventsReceived == 1);
	REQUIRE(objB->EventsReceived == 1);

	eventSource.Disconnect(objB, &EventObj::Callback);

	eventSource.EmitAnonymous();

	REQUIRE(objA->EventsReceived == 2);
	REQUIRE(objB->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Multiple Listeners On One Object", "Test if multiple listeners on one object work")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> objA = std::make_shared<EventObj>();

	eventSource.Connect(objA, &EventObj::Callback);
	eventSource.Connect(objA, &EventObj::AnotherCallback);

	REQUIRE(objA->EventsReceived == 0);

	eventSource.EmitAnonymous();

	REQUIRE(objA->EventsReceived == 3);

	eventSource.Disconnect(objA, &EventObj::Callback);

	eventSource.EmitAnonymous();

	REQUIRE(objA->EventsReceived == 5);
}

TEST_CASE("Nova/Core/Events/Event Stop Propagation", "Test if an event stops propagating to other listeners")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> objA = std::make_shared<EventObj>();
	Nova::Ref<EventObj> objB = std::make_shared<EventObj>(true);

	eventSource.Connect(objA, &EventObj::Callback);
	eventSource.Connect(objB, &EventObj::Callback);

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 0);

	eventSource.EmitAnonymous();

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Dead Event Listener", "Test if an event source prevents calling an event on a dead listener")
{
	Nova::EventSource<Nova::Event> eventSource;

	{
		Nova::Ref<EventObj> obj = std::make_shared<EventObj>();

		eventSource.Connect(obj, &EventObj::Callback);

		REQUIRE(eventSource.GetListenerCount() == 1);
	}

	eventSource.EmitAnonymous();

	REQUIRE(eventSource.GetListenerCount() == 0);
}
