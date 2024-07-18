#include "McmcFileLogEvent.hpp"
#include "AbstractParameter.hpp"
#include "AbstractLikelihood.hpp"
#include "AbstractPrior.hpp"
#include "Msg.hpp"
#include <string>
#include <vector>
#include <fstream>

McmcFileLogEvent::McmcFileLogEvent(void) :  prior(nullptr), likelihood(nullptr) {}

McmcFileLogEvent::McmcFileLogEvent(AbstractLikelihood* l, AbstractPrior* p, std::vector<std::pair<std::string, AbstractParameter*>> params) : prior(p), likelihood(l), parameters(params) {}

void McmcFileLogEvent::initialize() {
    if(prior == nullptr || likelihood == nullptr)
        Msg::error("LogFileEvent requires both the prior and the likelihood to be set!");
    if(file == "")
        Msg::error("The file for McmcFileLogEvent is not set!");

    std::fstream fs;
    fs.open (file, std::fstream::out);

    fs << "Iteration\tPrior\tLikelihood\tPosterior";
    for(std::pair<std::string, AbstractParameter*> entry : parameters)
        fs << "\t" << entry.first;
    fs << "\n";

    fs.close();
}

void McmcFileLogEvent::call(int iteration) {
    std::fstream fs;
    fs.open (file, std::fstream::app);

    double pVal = prior->lnPrior();
    double lVal = likelihood->lnLikelihood();

    fs << iteration << "\t" << pVal << "\t" << lVal << "\t" << pVal + lVal;
    for(std::pair<std::string, AbstractParameter*> entry : parameters)
        fs << "\t" << entry.second->writeValue();
    fs << "\n";

    fs.close();
}