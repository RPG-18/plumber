#pragma once

#include <kafka/AdminClient.h>

#include "AdminCommon.hpp"
#include "KafkaHelper.hpp"

namespace core {

class AdminClient : public kafka::clients::AdminClient
{
public:
    explicit AdminClient(const kafka::Properties &properties)
        : kafka::clients::AdminClient(properties)
    {}

    Optional<kafka::BrokerMetadata> fetchNodesMetadata(std::chrono::milliseconds timeout);

    ListGroupsResult listGroups(const std::string &group, std::chrono::milliseconds timeout);
};

inline Optional<kafka::BrokerMetadata> AdminClient::fetchNodesMetadata(
    std::chrono::milliseconds timeout)
{
    Optional<kafka::BrokerMetadata> ret;
    const rd_kafka_metadata_t *rk_metadata = nullptr;
    rd_kafka_resp_err_t err = rd_kafka_metadata(getClientHandle(),
                                                false,
                                                nullptr,
                                                &rk_metadata,
                                                convertMsDurationToInt(timeout));
    auto guard = kafka::rd_kafka_metadata_unique_ptr(rk_metadata);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        return ret;
    }

    kafka::BrokerMetadata metadata("undef");
    metadata.setOrigNodeName(
        rk_metadata->orig_broker_name ? std::string(rk_metadata->orig_broker_name) : "");

    for (int i = 0; i < rk_metadata->broker_cnt; ++i) {
        metadata.addNode(rk_metadata->brokers[i].id,
                         rk_metadata->brokers[i].host,
                         rk_metadata->brokers[i].port);
    }

    ret = metadata;
    return ret;
}

inline ListGroupsResult AdminClient::listGroups(const std::string &group,
                                                std::chrono::milliseconds timeout)
{
    const rd_kafka_group_list *rk_group_list = nullptr;
    rd_kafka_resp_err_t err = rd_kafka_list_groups(getClientHandle(),
                                                   group.empty() ? nullptr : group.c_str(),
                                                   &rk_group_list,
                                                   convertMsDurationToInt(timeout));
    auto guard = rd_kafka_group_list_unique_ptr(rk_group_list);

    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        return ListGroupsResult(kafka::Error{err, rd_kafka_err2str(err)});
    }

    std::vector<GroupInfo> groups;
    if (rk_group_list->group_cnt == 0) {
        return ListGroupsResult(groups);
    }

    groups.reserve(rk_group_list->group_cnt);

    for (int i = 0; i < rk_group_list->group_cnt; ++i) {
        auto groupItem = rk_group_list->groups[i];

        GroupInfo::Broker broker(groupItem.broker.id, groupItem.broker.host, groupItem.broker.port);

        GroupInfo info(broker);

        info.group = groupItem.group;
        info.error = kafka::Error{groupItem.err, rd_kafka_err2str(err)};
        info.state = groupItem.state;
        info.protocolType = groupItem.protocol_type;
        info.protocol = groupItem.protocol;

        std::vector<GroupInfo::Member> members;
        members.reserve(groupItem.member_cnt);
        for (int j = 0; j < groupItem.member_cnt; ++j) {
            auto memberItem = groupItem.members[j];
            GroupInfo::Member member;
            member.memberId = memberItem.member_id;
            member.clientId = memberItem.client_id;
            member.clientHost = memberItem.client_host;

            auto *metaPtr = static_cast<uint8_t *>(memberItem.member_metadata);
            member.metadata.insert(member.metadata.end(),
                                   metaPtr,
                                   metaPtr + memberItem.member_assignment_size);

            auto *assignmentPtr = static_cast<uint8_t *>(memberItem.member_assignment);
            member.assignment.insert(member.assignment.end(),
                                     assignmentPtr,
                                     assignmentPtr + memberItem.member_assignment_size);
            members.emplace_back(member);
        }

        info.members.swap(members);
        groups.emplace_back(info);
    }

    return ListGroupsResult(groups);
}
} // namespace core
