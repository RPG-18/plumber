import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "style.js" as Style
import "./LeftPanel" as LeftPanel
import kafkaui 1.0

Rectangle {
    id: mainScreen

    property var broker

    color: Style.Background

    Cluster {
        id: mainCluster

        broker: mainScreen.broker
    }

    RowLayout {
        spacing: 8
        anchors.fill: parent
        anchors.margins: 4

        LeftPanel.Panel {
            id: menu

            broker: mainScreen.broker
            Layout.fillHeight: true
            onActivatedItem: (indx) => {
                stackLayout.currentIndex = indx;
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            StackLayout {
                id: stackLayout

                anchors.fill: parent
                currentIndex: 0

                Overview {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onActivatedItem: (indx) => {
                        stackLayout.currentIndex = indx;
                        menu.setActiveItem(indx);
                    }
                }

                Brokers {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Topics {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

                Consumers {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }

            }

        }

    }

}
