import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import kafkaui 1.0
import "style.js" as Style
import "Components" as Components

Window {
    id: dialog

    property alias broker: topic.broker

    visible: true
    height: 280
    width: 800
    title: qsTr("Create New Topic")

    TopicCreator {
        id: topic

        name: name.text
        partitions: partitions.value
        replicationFactor: replication.value
        retention: retention.checked
        compaction: compaction.checked
    }

    Page {
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent

            GridLayout {
                anchors.fill: parent
                anchors.margins: 20
                columns: 2

                Text {
                    text: qsTr("Name")
                    Layout.preferredWidth: 130
                }

                TextField {
                    id: name

                    Layout.fillWidth: true
                    placeholderText: qsTr("My new Topic name")
                    selectByMouse: true
                }

                Text {
                    text: qsTr("Partitions")
                }

                SpinBox {
                    id: partitions

                    from: 1
                    stepSize: 1
                    value: 3
                    Layout.preferredWidth: 60
                }

                Text {
                    text: qsTr("Replication Factor")
                }

                SpinBox {
                    id: replication

                    from: 1
                    to: mainCluster.brokerModel().brokers
                    Layout.preferredWidth: 60
                    stepSize: 1
                    value: 1
                }

                Text {
                    text: qsTr("Cleanup Policy")
                }

                RowLayout {
                    Text {
                        text: qsTr("Retention (time or size)")
                    }

                    Switch {
                        id: retention

                        checked: true
                        onCheckedChanged: {
                            if (!retention.checked)
                                compaction.checked = true;

                        }
                    }

                    Text {
                        text: qsTr("Compaction (key-based)")
                    }

                    Switch {
                        id: compaction

                        onCheckedChanged: {
                            if (!compaction.checked)
                                retention.checked = true;

                        }
                    }

                }

                Item {
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

            }

        }

        footer: Rectangle {
            width: dialog.width
            implicitHeight: createButton.height + 16

            RowLayout {
                anchors.fill: parent
                anchors.margins: 8

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: createButton

                    enabled: name.length > 0
                    text: qsTr("CREATE TOPIC")
                    onClicked: {
                        let e = topic.create();
                        if (e.isError) {
                            err.show(e);
                            return ;
                        }
                        mainCluster.topicModel().refresh();
                        dialog.close();
                    }
                }

            }

        }

    }

    Components.ErrorDialog {
        id: err

        anchors.centerIn: parent
        width: parent.width * 0.8
    }

}
