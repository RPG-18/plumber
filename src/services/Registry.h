#pragma once

#include <memory>

#include "ConfigurationService.h"

class Registry
{
public:
    static Registry *instance() noexcept;

    ConfigurationService *configuration();
    void setConfiguration(std::shared_ptr<ConfigurationService> service);

private:
    std::shared_ptr<ConfigurationService> m_configuration;
};

#define Services Registry::instance()