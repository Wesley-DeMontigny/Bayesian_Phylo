#include "McmcFileLogEvent.hpp"
#include "modeling/ModelNode.hpp"
#include "core/Msg.hpp"
#include <string>
#include <vector>
#include <fstream>

McmcFileLogEvent::McmcFileLogEvent(std::vector<std::pair<std::string, ModelNode*>> n, std::string f) : nodes(n), file(f) {}

void McmcFileLogEvent::initialize() {
    std::fstream fs;
    fs.open (file, std::fstream::out);

    for(std::pair<std::string, ModelNode*> entry : nodes)
        fs << "\t" << entry.first;
    fs << "\n";

    fs.close();
}

void McmcFileLogEvent::call(int iteration) {
    std::fstream fs;
    fs.open (file, std::fstream::app);

    for(std::pair<std::string, ModelNode*> entry : nodes)
        fs << "\t" << entry.second->writeValue();
    fs << "\n";

    fs.close();
}