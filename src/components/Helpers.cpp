#include <QtQml/QQmlEngine>

#include "Helpers.h"

#include "AvroOption.h"
#include "Cluster.h"
#include "ConfigModel.h"
#include "Consumer.h"
#include "ConsumerHelperModels.h"
#include "ConsumerModel.h"
#include "ExportImportFabric.h"
#include "HexView.h"
#include "KafkaConnectivityTester.h"
#include "Producer.h"
#include "ProtoOption.h"
#include "Settings.h"
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
    qmlRegisterType<Settings>("plumber", 1, 0, "Settings");

    qmlRegisterType<Producer>("plumber", 1, 0, "Producer");
    qmlRegisterType<ProducerOptions>("plumber", 1, 0, "ProducerOptions");
    qmlRegisterType<ProtoOption>("plumber", 1, 0, "ProtoOption");
    qmlRegisterType<AvroOption>("plumber", 1, 0, "AvroOption");

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
    qmlRegisterAnonymousType<ConsumerModel>("plumber", 1);

    qmlRegisterType<HexView>("plumber", 1, 0, "HexView");
    qmlRegisterType<FileDataSource>("plumber", 1, 0, "FileDataSource");
    qmlRegisterType<MessageValueDataSource>("plumber", 1, 0, "MessageValueDataSource");

    qmlRegisterAnonymousType<AbstractDataSource>("HexView", 1);

    qmlRegisterType<ExportImportFabric>("plumber", 1, 0, "ExportImportFabric");
    qmlRegisterType<ConsumerFilterModel>("plumber", 1, 0, "ConsumerFilterModel");
    qmlRegisterType<ConsumerGroupInfoItem>("plumber", 1, 0, "ConsumerGroupInfoItem");
    qmlRegisterAnonymousType<GroupMemberModel>("plumber", 1);
}