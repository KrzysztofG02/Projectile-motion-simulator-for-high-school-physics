#ifndef EVENTSHANDLER_H
#define EVENTSHANDLER_H

#include <SFML/Window/Event.hpp>

#include <functional>
#include <list>
#include <unordered_map>
#include <vector>


namespace gs
{

class EventsHandlerWithAccess;


class EventsHandler
{
private:
    friend class EventsHandlerWithAccess;

    static inline std::unordered_map<sf::Event::EventType, std::list<std::function<void (sf::Event const &)>>> staticEventHandlersMap = {};
    std::unordered_map<sf::Event::EventType, std::vector<std::list<std::function<void (sf::Event const &)>>::iterator>> mEventHandlersMap;


public:
    ~EventsHandler();


protected:
    void addEvent(sf::Event::EventType const &eventType, std::function<void (sf::Event const &)> const &func);


private:
    void addEventToMemberMap(sf::Event::EventType const &eventType, std::list<std::function<void (sf::Event const &)>>::iterator const &it);
};


class EventsHandlerWithAccess
{
private:
    friend class App;

    static void handleEvent(sf::Event const &event);
};

}

#endif // EVENTSHANDLER_H
