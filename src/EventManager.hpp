#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP
#include <vector>
#include <unordered_map>

class AbstractEvent;

class EventManager {
    public:
        EventManager(void);
        void registerEvent(AbstractEvent* e, int interval);
        void initialize();
        void call(int iteration);
    private:
        std::unordered_map<int, std::vector<AbstractEvent*>> events;
};

#endif