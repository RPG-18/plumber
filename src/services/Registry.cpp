#include "Registry.h"

ConfigurationService *Registry::configuration()
{
    return m_configuration.get();
}

void Registry::setConfiguration(std::shared_ptr<ConfigurationService> service)
{
    m_configuration = service;
}

Registry *Registry::instance() noexcept
{
    static Registry obj;
    return &obj;
}
