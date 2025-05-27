#include "EventsHandler.h"


using namespace std;


gs::EventsHandler::~EventsHandler()
{
    for(auto memberMap_constIt = mEventHandlersMap.cbegin(); memberMap_constIt != mEventHandlersMap.cend(); ++memberMap_constIt)
    {
        auto &funcListOfStaticMap = staticEventHandlersMap[memberMap_constIt->first];

        for(auto list_constIt = memberMap_constIt->second.cbegin(); list_constIt != memberMap_constIt->second.cend();)
        {
            funcListOfStaticMap.erase(*(list_constIt++));
        }

        if(funcListOfStaticMap.empty())
        {
            staticEventHandlersMap.erase(memberMap_constIt->first);
        }
    }
}


void gs::EventsHandlerWithAccess::handleEvent(sf::Event const &event)
{
    auto const eventHandlersMap_it = gs::EventsHandler::staticEventHandlersMap.find(event.type);

    if(eventHandlersMap_it != gs::EventsHandler::staticEventHandlersMap.end())
    {
        for(function<void (sf::Event const &)> &func : eventHandlersMap_it->second)
        {
            func(event);
        }
    }
}

void gs::EventsHandler::addEvent(sf::Event::EventType const &eventType, std::function<void (sf::Event const &)> const &func)
{
    auto eventHandlersMap_it = staticEventHandlersMap.find(eventType);

    if(eventHandlersMap_it == staticEventHandlersMap.end())
    {
        eventHandlersMap_it = staticEventHandlersMap.insert(make_pair(eventType, list<function<void (sf::Event const &)>>{})).first;
    }

    std::list<std::function<void (sf::Event const &)>>::iterator const it =
        eventHandlersMap_it->second.insert(eventHandlersMap_it->second.end(), func);

    this->addEventToMemberMap(eventType, it);
}


void gs::EventsHandler::addEventToMemberMap(sf::Event::EventType const &eventType, std::list<std::function<void (sf::Event const &)>>::iterator const &it)
{
    auto const eventHandlersMap_it = mEventHandlersMap.find(eventType);

    if(eventHandlersMap_it == mEventHandlersMap.end())
    {
        mEventHandlersMap[eventType] = vector<list<function<void (sf::Event const &)>>::iterator>{it};
    }
    else
    {
        eventHandlersMap_it->second.emplace_back(it);
    }
}
