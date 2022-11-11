#include "KafkaAdmin.h"
#include "AdminClient.hpp"
#include "spdlog/spdlog.h"

namespace core {

KafkaAdmin::KafkaAdmin(ClusterConfig cfg, QObject *parent)
    : QObject(parent)
    , m_cfg(std::move(cfg))
{}

std::tuple<KafkaAdmin::Topics, Error> KafkaAdmin::listTopics(std::chrono::milliseconds timeout)
{
    using namespace kafka::clients::admin;

    const QString when("list topic");

    try {
        Config cfg(m_cfg.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());
        AdminClient client(cfg);

        const auto response = client.listTopics(timeout);
        if (!response.error) {
            Topics topics;
            topics.reserve(qsizetype(response.topics.size()));
            for (const auto &topic : response.topics) {
                topics.emplaceBack(QString::fromStdString(topic));
            }
            return std::make_tuple(topics, Error{});
        }

        spdlog::error("list topic error: {}", response.error.message());
        return std::make_tuple(Topics{},
                               Error{when, QString::fromStdString(response.error.message())});

    } catch (const kafka::KafkaException &e) {
        spdlog::error("unexpected exception caught: {}", e.what());
        return std::make_tuple(Topics{}, Error(when, e.what()));
    }
}

std::optional<Error> KafkaAdmin::deleteTopics(const Topics &topics,
                                              std::chrono::milliseconds timeout)
{
    using namespace kafka::clients::admin;

    const QString when("topic remove");

    try {
        Config cfg(m_cfg.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        kafka::clients::AdminClient client(cfg);
        kafka::Topics toDelete;
        for (const auto &topic : topics) {
            toDelete.insert(topic.toStdString());
        }

        const auto result = client.deleteTopics(toDelete, timeout);
        if (result.error) {
            spdlog::error("topic remove error {}", result.error.message());
            return Error{when, QString::fromStdString(result.error.message())};
        }
    } catch (const kafka::KafkaException &e) {
        spdlog::error("topic create exception {}", e.what());
        return Error{when, QString::fromStdString(e.what())};
    }

    return {};
}

std::tuple<std::optional<KafkaAdmin::BrokerMetadata>, Error> KafkaAdmin::fetchNodesMetadata(
    std::chrono::milliseconds timeout)
{
    using namespace kafka::clients::admin;

    const QString when("topic remove");

    try {
        Config cfg(m_cfg.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        core::AdminClient client(cfg);

        const auto md = client.fetchNodesMetadata(timeout);
        return std::make_tuple(md, Error{});

    } catch (const kafka::KafkaException &e) {
        spdlog::error("fetch nodes metadata exception {}", e.what());
        return std::make_tuple(std::nullopt, Error{when, QString::fromStdString(e.what())});
    }
}

std::optional<Error> KafkaAdmin::createTopics(const Topics &topics,
                                              int numPartitions,
                                              int replicationFactor,
                                              const kafka::Properties &topicConfig,
                                              std::chrono::milliseconds timeout)
{
    using namespace kafka::clients::admin;

    const QString when("create topic");
    try {
        Config cfg(m_cfg.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());

        kafka::Topics t;
        for (const auto &topic : topics) {
            t.insert(topic.toStdString());
        }

        core::AdminClient client(cfg);
        const auto res = client.createTopics(t,
                                             numPartitions,
                                             replicationFactor,
                                             topicConfig,
                                             timeout);
        if (res.error) {
            return Error{when, QString::fromStdString(res.error.message())};
        }
    } catch (const kafka::KafkaException &e) {
        spdlog::error("topic create exception {}", e.what());
        return Error{when, QString::fromStdString(e.what())};
    }
    return {};
}

std::tuple<std::vector<GroupInfo>, Error> KafkaAdmin::listGroups(const QString &group,
                                                                 std::chrono::milliseconds timeout)
{
    using namespace kafka::clients::admin;
    const QString when("list group");

    try {
        Config cfg(m_cfg.properties->map());
        cfg.put(Config::BOOTSTRAP_SERVERS, m_cfg.bootstrap.toStdString());
        AdminClient client(cfg);

        const auto groupFilter = group.toStdString();
        const auto result = client.listGroups(groupFilter, timeout);
        if (result.error) {
            spdlog::error("list group error {}", result.error.toString());
            return std::make_tuple(std::vector<GroupInfo>(),
                                   Error(when, QString::fromStdString(result.error.message())));
        }

        return std::make_tuple(result.groups, Error());
    } catch (const kafka::KafkaException &e) {
        spdlog::error("unexpected exception caught: {}", e.what());
        return std::make_tuple(std::vector<GroupInfo>(), Error(when, e.what()));
    }
}

} // namespace core