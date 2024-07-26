#ifndef MCMC_SCREEN_LOG_EVENT
#define MCMC_SCREEN_LOG_EVENT
#include "Event.hpp"
#include <string>
#include <vector>

class ModelNode;

class McmcScreenLogEvent : public Event{
    public:
        McmcScreenLogEvent(void)=delete;
        McmcScreenLogEvent(std::vector<std::pair<std::string, ModelNode*>> n);
        void initialize();
        void call(int iteration);
    private:
        std::vector<std::pair<std::string, ModelNode*>> nodes;
};

#endif