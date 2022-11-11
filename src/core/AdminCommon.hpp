#pragma once

#include <kafka/Error.h>
#include <vector>
#include "GroupInfo.hpp"

namespace core {
/**
 * The result of AdminClient::listGroups().
 */
struct ListGroupsResult
{
    explicit ListGroupsResult(const kafka::Error& err): error(err) {}
    explicit ListGroupsResult(std::vector<GroupInfo> groups): groups(std::move(groups)) {}

    /**
     * The result error
     */
    kafka::Error error;

    /**
     * List for groups
     */
    std::vector<GroupInfo> groups;
};

}