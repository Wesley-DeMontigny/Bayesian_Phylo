#include "Node.hpp"

Node::Node() : index(0), ancestor(nullptr), name(""), isTip(false), offset(0), activeCL(0), activeTP(0), needsUpdate(false) {}