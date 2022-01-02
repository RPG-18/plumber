#pragma once

#include <QtCore/QObject>

class QSettings;

class Settings : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString leftPanelState READ leftPanelState WRITE setLeftPanelState NOTIFY
                   leftPanelStateChanged)
    Settings(QObject *parent = nullptr);
    ~Settings();

    QString leftPanelState();
    void setLeftPanelState(const QString &value);

signals:

    void leftPanelStateChanged();

private:
    QSettings *m_settings;
};
