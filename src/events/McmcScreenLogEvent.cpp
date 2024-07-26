#include "McmcScreenLogEvent.hpp"
#include "modeling/ModelNode.hpp"
#include "core/Msg.hpp"
#include <string>
#include <vector>
#include <iostream>

McmcScreenLogEvent::McmcScreenLogEvent(std::vector<std::pair<std::string, ModelNode*>> n) : nodes(n) {}

void McmcScreenLogEvent::initialize() {
    for(std::pair<std::string, ModelNode*> entry : nodes)
        std::cout << "\t" << entry.first;
    std::cout << std::endl;
}

void McmcScreenLogEvent::call(int iteration) {
    for(std::pair<std::string, ModelNode*> entry : nodes)
        std::cout << "\t" << entry.second->writeValue();
    std::cout << std::endl;
}