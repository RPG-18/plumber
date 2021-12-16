#include <QtQml/QQmlEngine>

#include "Helpers.h"

#include "Cluster.h"
#include "ConfigModel.h"
#include "Consumer.h"
#include "ConsumerHelperModels.h"
#include "KafkaConnectivityTester.h"
#include "Producer.h"
#include "ProtoOption.h"
#include "TopicCreator.h"

void registerTypes()
{
    qmlRegisterType<KafkaConnectivityTester>("plumber", 1, 0, "ConnectivityTester");
    qmlRegisterType<ConfigModel>("plumber", 1, 0, "ConfigModel");
    qmlRegisterType<Cluster>("plumber", 1, 0, "Cluster");
    qmlRegisterType<TopicFilterModel>("plumber", 1, 0, "TopicFilterModel");
    qmlRegisterType<Consumer>("plumber", 1, 0, "Consumer");
    qmlRegisterType<ConsumerTypeSelector>("plumber", 1, 0, "ConsumerTypeSelector");
    qmlRegisterType<ConsumerLimiterSelector>("plumber", 1, 0, "ConsumerLimiterSelector");
    qmlRegisterType<ConsumerFilterSelector>("plumber", 1, 0, "ConsumerFilterSelector");
    qmlRegisterType<ConsumerBeginningSelector>("plumber", 1, 0, "ConsumerBeginningSelector");
    qmlRegisterType<TopicCreator>("plumber", 1, 0, "TopicCreator");

    qmlRegisterType<Producer>("plumber", 1, 0, "Producer");
    qmlRegisterType<ProducerOptions>("plumber", 1, 0, "ProducerOptions");
    qmlRegisterType<ProtoOption>("plumber", 1, 0, "ProtoOption");

    qmlRegisterType<ConsumerTypesModel>("plumber", 1, 0, "ConsumerTypesModel");
    qmlRegisterType<CustomTypesModel>("plumber", 1, 0, "CustomTypesModel");
    qmlRegisterType<StartFromTimeBasedModel>("plumber", 1, 0, "StartFromTimeBasedModel");
    qmlRegisterType<FiltersModel>("plumber", 1, 0, "FiltersModel");
    qmlRegisterType<HidePrivateTopicModel>("plumber", 1, 0, "HidePrivateTopicModel");

    qmlRegisterType<LimitModel>("plumber", 1, 0, "LimitModel");
    qmlRegisterType<MessageWrapper>("plumber", 1, 0, "MessageWrapper");
    qmlRegisterType<HeaderTableModel>("plumber", 1, 0, "HeaderTableModel");

    qmlRegisterAnonymousType<BrokerModel>("plumber", 1);
    qmlRegisterAnonymousType<TopicModel>("plumber", 1);
    qmlRegisterAnonymousType<MessageModel>("plumber", 1);
}