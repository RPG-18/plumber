#pragma once

#include <kafka/AdminClient.h>


namespace core {
class AdminClient : public kafka::AdminClient
{
public:
    explicit AdminClient(const kafka::Properties &properties)
        : kafka::AdminClient(properties)
    {}

    Optional<kafka::BrokerMetadata> fetchNodesMetadata(std::chrono::milliseconds timeout,
                                                       bool disableErrorLogging = false);
};

inline Optional<kafka::BrokerMetadata> AdminClient::fetchNodesMetadata(
    std::chrono::milliseconds timeout, bool disableErrorLogging)
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
}