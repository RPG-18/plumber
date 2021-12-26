import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "Components" as Components
import "style.js" as Style
import "./LeftPanel" as LeftPanel
import "pages.js" as Pages
import plumber

Rectangle {
    id: mainScreen

    property var broker

    color: Style.Background

    Cluster {
        id: mainCluster

        broker: mainScreen.broker
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            Components.Button {
                text: "PRODUCER"
                onClicked: {
                    Pages.createProducerScreen(mainCluster.topicModel(), mainScreen.broker);
                }
            }

        }

        RowLayout {
            spacing: 8
            Layout.fillWidth: true
            Layout.fillHeight: true

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

}
