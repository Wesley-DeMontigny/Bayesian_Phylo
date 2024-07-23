#ifndef MCMC_FILE_LOG_EVENT
#define MCMC_FILE_LOG_EVENT
#include "AbstractEvent.hpp"
#include <string>
#include <vector>

class AbstractLikelihood;
class AbstractPrior;
class AbstractParameter;

class McmcFileLogEvent : public AbstractEvent{
    public:
        McmcFileLogEvent(void)=delete;
        McmcFileLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::string f);
        McmcFileLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::vector<std::pair<std::string, AbstractParameter*>> params, std::string f);
        void initialize();
        void call(int iteration);
    private:
        AbstractLikelihood* likelihood;
        AbstractPrior* prior;
        std::vector<std::pair<std::string, AbstractParameter*>> parameters;
        std::string file;
};

#endif