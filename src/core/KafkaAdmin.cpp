#include "KafkaAdmin.h"
#include "AdminClient.hpp"
#include "spdlog/spdlog.h"

namespace core {

KafkaAdmin::Error::Error()
    : isError(false)
{}

KafkaAdmin::Error::Error(QString wh, QString wt)
    : isError(true)
    , where(std::move(wh))
    , what(std::move(wt))
{}

KafkaAdmin::KafkaAdmin(ClusterConfig cfg, QObject *parent)
    : QObject(parent)
    , m_cfg(std::move(cfg))
{}

std::tuple<KafkaAdmin::Topics, KafkaAdmin::Error> KafkaAdmin::listTopics(
    std::chrono::milliseconds timeout)
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
            topics.reserve(response.topics.size());
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

std::optional<KafkaAdmin::Error> KafkaAdmin::deleteTopics(const Topics &topics,
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

        auto result = client.deleteTopics(toDelete, timeout);
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

} // namespace core