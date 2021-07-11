#include <spdlog/spdlog.h>

#include "KafkaUtility.h"

void KafkaSpdLogger(int level, const char * /*filename*/, int /*lineno*/, const char *msg)
{
    switch (level) {
    case 0:
    case 1:
    case 2:
        spdlog::critical(msg);
        break;
    case 3:
        spdlog::error(msg);
        break;
    case 4:
        spdlog::warn(msg);
        break;
    case 5:
    case 6:
        spdlog::info(msg);
        break;
    case 7:
    case 8:
        spdlog::debug(msg);
        break;
    default:
        spdlog::debug(msg);
    }
}