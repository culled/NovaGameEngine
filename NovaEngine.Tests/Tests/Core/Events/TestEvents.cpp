#include <catch.hpp>

#include "EventObj.h"
#include <Nova/Core/Events/EventSource.h>

TEST_CASE("Nova/Core/Events/Call Event", "Test if an event listener gets notified when an event is called")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> obj = Nova::MakeRef<EventObj>();

	eventSource.Connect(obj, &EventObj::Callback);

	REQUIRE(obj->EventsReceived == 0);

	eventSource.Emit(Nova::MakeRef<Nova::Event>());

	REQUIRE(obj->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Disconnect Event", "Test if an event listener disconnects properly")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> objA = Nova::MakeRef<EventObj>();
	Nova::Ref<EventObj> objB = Nova::MakeRef<EventObj>();

	eventSource.Connect(objA, &EventObj::Callback);
	eventSource.Connect(objB, &EventObj::Callback);

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 0);

	eventSource.Emit(Nova::MakeRef<Nova::Event>());

	REQUIRE(objA->EventsReceived == 1);
	REQUIRE(objB->EventsReceived == 1);

	eventSource.Disconnect(objB, &EventObj::Callback);

	eventSource.Emit(Nova::MakeRef<Nova::Event>());

	REQUIRE(objA->EventsReceived == 2);
	REQUIRE(objB->EventsReceived == 1);
}

TEST_CASE("Nova/Core/Events/Event Stop Propagation", "Test if an event stops propagating to other listeners")
{
	Nova::EventSource<Nova::Event> eventSource;

	Nova::Ref<EventObj> objA = Nova::MakeRef<EventObj>();
	Nova::Ref<EventObj> objB = Nova::MakeRef<EventObj>(true);

	eventSource.Connect(objA, &EventObj::Callback);
	eventSource.Connect(objB, &EventObj::Callback);

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 0);

	eventSource.Emit(Nova::MakeRef<Nova::Event>());

	REQUIRE(objA->EventsReceived == 0);
	REQUIRE(objB->EventsReceived == 1);
}