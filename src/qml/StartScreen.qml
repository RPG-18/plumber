import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import "style.js" as Style
import "pages.js" as Pages
import kafkaui 1.0

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: menuArea

            Layout.fillWidth: true
            Layout.minimumWidth: 50
            Layout.preferredWidth: 100
            Layout.maximumWidth: 300
            Layout.minimumHeight: 150
            Layout.fillHeight: true
            color: "white"

            ConnectivityTester {
                id: tester

                onIsTested: {
                    busy.running = false;
                    Pages.createMainScreen(brokerView.selectedBroker);
                }
            }

            ListView {
                id: brokerView

                property var selectedBroker

                anchors.fill: parent
                anchors.margins: 4
                boundsMovement: Flickable.StopAtBounds
                spacing: 4

                model: ConfigModel {
                    id: brokerModel
                }

                delegate: BrokerDelegate {
                    width: menuArea.width
                    brokerColor: bgcolor
                    brokerName: name
                    brokerBootstrap: bootstrap
                    onRemove: {
                        brokerModel.removeBroker(index);
                    }
                    onOpen: {
                        busy.running = true;
                        brokerView.selectedBroker = brokerModel.broker(index);
                        tester.testBroker(brokerModel.broker(index));
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }

            }

            Rectangle {
                width: 2
                height: parent.height
                color: Style.BorderColor
                anchors.right: parent.right
            }

        }

        Rectangle {
            color: Style.Background
            Layout.fillWidth: true
            Layout.minimumWidth: 100
            Layout.preferredWidth: 200
            Layout.preferredHeight: 100
            Layout.fillHeight: true

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("New Kafka Cluster")
                onClicked: {
                    dialog.open();
                }
            }

        }

    }

    ClusterConfigurationDialog {
        id: dialog

        onCreateBroker: (name, bootstrap, color,  properties) => {
            brokerModel.createBroker(name, bootstrap, color, properties);
        }
    }

    BusyIndicator {
        id: busy

        anchors.centerIn: parent
        running: false
    }

}
