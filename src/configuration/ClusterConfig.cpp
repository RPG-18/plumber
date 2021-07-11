#include <kafka/Properties.h>

#include "ClusterConfig.h"

ClusterConfig::ClusterConfig()
    : properties(std::make_shared<kafka::Properties>())
{}