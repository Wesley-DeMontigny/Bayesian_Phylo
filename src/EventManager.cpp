#include "EventManager.hpp"
#include "AbstractEvent.hpp"
#include <algorithm>

EventManager::EventManager(void){}


void EventManager::registerEvent(AbstractEvent* e, int interval){
    events[interval].push_back(e);
}

void EventManager::initialize(){
    for (auto& [interval, eventVector] : events) 
        for (auto& event : eventVector)
            event->initialize();
}

void EventManager::call(int iteration) {
    for (auto& [interval, eventVector] : events) {
        if (iteration % interval == 0) {
            for (auto& event : eventVector) {
                event->call(iteration);
            }
        }
    }
}