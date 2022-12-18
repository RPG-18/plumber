#include <QtCore/QtEndian>

#include "GroupInfo.h"
#include "spdlog/spdlog.h"

namespace core {
GroupInfo::GroupInfo(Broker broker)
    : broker(std::move(broker)){

    };

MemberAssignmentInformation::MemberAssignmentInformation(const std::vector<uint8_t> &data)
{
    // Version + topic list size
    if (data.size() < sizeof(uint16_t) + sizeof(uint32_t)) {
        spdlog::error("parse member assignment information: message is malformed");
        return;
    }
    const uint8_t *ptr = data.data();
    const uint8_t *end = ptr + data.size();
    memcpy(&m_version, ptr, sizeof(m_version));
    m_version = qFromBigEndian(m_version);
    ptr += sizeof(m_version);

    uint32_t total_topics;
    memcpy(&total_topics, ptr, sizeof(total_topics));
    total_topics = qFromBigEndian(total_topics);
    ptr += sizeof(total_topics);

    for (uint32_t i = 0; i != total_topics; ++i) {
        if (ptr + sizeof(uint16_t) > end) {
            spdlog::error("parse member assignment information: message is malformed");
            return;
        }
        uint16_t topic_length;
        memcpy(&topic_length, ptr, sizeof(topic_length));
        topic_length = qFromBigEndian(topic_length);
        ptr += sizeof(topic_length);

        // Check for string length + size of partitions list
        if (topic_length > std::distance(ptr, end) + sizeof(uint32_t)) {
            spdlog::error("parse member assignment information: message is malformed");
            return;
        }
        std::string topic_name(ptr, ptr + topic_length);
        ptr += topic_length;

        uint32_t total_partitions;
        memcpy(&total_partitions, ptr, sizeof(total_partitions));
        total_partitions = qFromBigEndian(total_partitions);
        ptr += sizeof(total_partitions);

        if (ptr + total_partitions * sizeof(uint32_t) > end) {
            spdlog::error("parse member assignment information: message is malformed");
            return;
        }
        for (uint32_t j = 0; j < total_partitions; ++j) {
            uint32_t partition;
            memcpy(&partition, ptr, sizeof(partition));
            partition = qFromBigEndian(partition);
            ptr += sizeof(partition);

            m_topic_partitions.insert(std::make_pair(topic_name, partition));
        }
    }
}

uint16_t MemberAssignmentInformation::version() const
{
    return m_version;
}

const kafka::TopicPartitions &MemberAssignmentInformation::topicPartitions() const
{
    return m_topic_partitions;
}
} // namespace core