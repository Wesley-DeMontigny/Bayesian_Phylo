#ifndef MCMC_FILE_LOG_EVENT
#define MCMC_FILE_LOG_EVENT
#include "Event.hpp"
#include <string>
#include <vector>

class ModelNode;

class McmcFileLogEvent : public Event{
    public:
        McmcFileLogEvent(void)=delete;
        McmcFileLogEvent(std::vector<std::pair<std::string, ModelNode*>> n, std::string f);
        void initialize();
        void call(int iteration);
    private:
        std::vector<std::pair<std::string, ModelNode*>> nodes;
        std::string file;
};

#endif