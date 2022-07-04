#include "Registry.h"

ConfigurationService *Registry::configuration()
{
    return m_configuration.get();
}

void Registry::setConfiguration(std::shared_ptr<ConfigurationService> service)
{
    m_configuration = std::move(service);
}

Registry *Registry::instance() noexcept
{
    static Registry obj;
    return &obj;
}

ErrorsService &Registry::errors()
{
    return *m_errors.get();
}

void Registry::setErrors(std::shared_ptr<ErrorsService> service)
{
    m_errors = std::move(service);
}