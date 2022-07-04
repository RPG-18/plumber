#pragma once

#include <memory>

#include "ConfigurationService.h"
#include "ErrorsService.h"

class Registry
{
public:
    static Registry *instance() noexcept;

    ConfigurationService *configuration();
    void setConfiguration(std::shared_ptr<ConfigurationService> service);

    ErrorsService &errors();
    void setErrors(std::shared_ptr<ErrorsService>);

private:
    std::shared_ptr<ConfigurationService> m_configuration;
    std::shared_ptr<ErrorsService> m_errors;
};

#define Services Registry::instance()