#pragma once

#include <kafka/Error.h>

namespace core {
/**
 * The group info
 */
struct GroupInfo
{
    struct Broker
    {
    public:
        using Id = int;
        using Host = std::string;
        using Port = int;

        Broker(Id i, Host h, Port p)
            : id(i)
            , host(std::move(h))
            , port(p)
        {}

        /**
         * The node id.
         */
        Broker::Id id;

        /**
         * The host name.
         */
        Broker::Host host;

        /**
         * The port.
         */
        Broker::Port port;

        /**
         * Obtains explanatory string.
         */
        std::string toString() const
        {
            return host + ":" + std::to_string(port) + "/" + std::to_string(id);
        }
    };

    /**
   * Originating broker info
   */
    Broker broker;

    /**
   * The group name
   */
    std::string group;

    /**
   * Broker-originated error
   */
    kafka::Error error;

    std::string state;

    std::string protocolType;
    std::string protocol;

    struct Member
    {
        using Host = std::string;

        std::string memberId;
        std::string clientId;
        Member::Host clientHost;
        std::vector<uint8_t> metadata;
        std::vector<uint8_t> assignment;
    };

    std::vector<Member> members;

    explicit GroupInfo(Broker broker)
        : broker(std::move(broker)){};
};
} // namespace core