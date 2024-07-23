#ifndef MCMC_SCREEN_LOG_EVENT
#define MCMC_SCREEN_LOG_EVENT
#include "AbstractEvent.hpp"
#include <string>
#include <vector>

class AbstractLikelihood;
class AbstractPrior;
class AbstractParameter;

class McmcScreenLogEvent : public AbstractEvent{
    public:
        McmcScreenLogEvent(void)=delete;
        McmcScreenLogEvent(AbstractLikelihood* l, AbstractPrior* p);
        McmcScreenLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::vector<std::pair<std::string, AbstractParameter*>> params);
        void initialize();
        void call(int iteration);
    private:
        AbstractLikelihood* likelihood;
        AbstractPrior* prior;
        std::vector<std::pair<std::string, AbstractParameter*>> parameters;
};

#endif