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
        McmcFileLogEvent(void);
        McmcFileLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::vector<std::pair<std::string, AbstractParameter*>> params);
        void initialize();
        void call(int iteration);
        void setLikelihood(AbstractLikelihood* l) {likelihood = l;}
        void setPrior(AbstractPrior* p) {prior = p;}
        void addParameter(std::string name, AbstractParameter* p) {parameters.push_back(std::make_pair(name, p));}
        void setFile(std::string dir) {file = dir;}
    private:
        AbstractLikelihood* likelihood;
        AbstractPrior* prior;
        std::vector<std::pair<std::string, AbstractParameter*>> parameters;
        std::string file;
};

#endif