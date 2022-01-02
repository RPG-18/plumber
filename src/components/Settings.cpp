#include <QtCore/QSettings>

#include "Settings.h"

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_settings(new QSettings(QSettings::UserScope, this))
{}

Settings::~Settings()
{
    m_settings->sync();
}

QString Settings::leftPanelState()
{
    m_settings->beginGroup("left_panel");
    const auto state = m_settings->value("state", "default");
    m_settings->endGroup();
    return state.toString();
}

void Settings::setLeftPanelState(const QString &value)
{
    m_settings->beginGroup("left_panel");
    m_settings->setValue("state", value);
    m_settings->endGroup();
    emit leftPanelStateChanged();
}