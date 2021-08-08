#pragma once

#include <QtCore/QObject>
#include <QtCore/QVariant>

class KafkaConnectivityTester : public QObject
{
    Q_OBJECT
public:
    explicit KafkaConnectivityTester(QObject *parent = nullptr);

    Q_INVOKABLE void test(const QString &url, const QString &props) noexcept;
    Q_INVOKABLE void testBroker(const QVariant &variant) noexcept;

signals:

    void isTested(bool result);

private slots:
    void tested(bool result);
};
