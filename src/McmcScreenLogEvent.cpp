#include "McmcScreenLogEvent.hpp"
#include "AbstractParameter.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractPrior.hpp"
#include "Msg.hpp"
#include <string>
#include <vector>
#include <iostream>

McmcScreenLogEvent::McmcScreenLogEvent(AbstractLikelihood* l, AbstractPrior* p) : prior(p), likelihood(l) {}


McmcScreenLogEvent::McmcScreenLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::vector<std::pair<std::string, AbstractParameter*>> params) : prior(p), likelihood(l), parameters(params) {}

void McmcScreenLogEvent::initialize() {
    if(prior == nullptr || likelihood == nullptr)
        Msg::error("McmcScreenLogEvent requires both the prior and the likelihood to be provided");

    std::cout << "Iteration\tPrior\tLikelihood\tPosterior";
    for(std::pair<std::string, AbstractParameter*> entry : parameters)
        std::cout << "\t" << entry.first;
    std::cout << std::endl;
}

void McmcScreenLogEvent::call(int iteration) {
    double pVal = prior->lnPrior();
    double lVal = likelihood->lnLikelihood();

    std::cout << iteration << "\t" << pVal << "\t" << lVal << "\t" << pVal + lVal;
    for(std::pair<std::string, AbstractParameter*> entry : parameters)
        std::cout << "\t" << entry.second->writeValue();
    std::cout << std::endl;
}