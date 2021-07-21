import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import kafkaui 1.0
import "style.js" as Style

Window {
    id: dialog

    property string propertiesPlaceholder: "# librdkafka properties \n
ssl.protocol=...
ssl.enabled.protocols=...
"

    signal createBroker(string name, string bootstrap, string color, string properties)

    function open() {
        if (dialog.visible)
            return ;

        let startX = appWindow.x + appWindow.width / 2 - dialog.width / 2;
        let startY = appWindow.y + appWindow.height / 2 - dialog.height / 2;
        dialog.x = startX;
        dialog.y = startY;
        clusterName.text = "";
        bootstrapServers.text = "";
        addProperties.text = "";
        colorBox.currentIndex = 0;
        dialog.show();
    }

    function clearAndClose() {
        dialog.close();
    }

    height: 605
    width: 930
    title: qsTr("Cluster Configuration")
    color: Style.Background

    Page {
        id: page

        anchors.fill: parent
        state: "default"

        Item {
            anchors.fill: parent
            anchors.margins: 20

            ColumnLayout {
                anchors.fill: parent

                TabBar {
                    id: bar

                    height: 25
                    Layout.fillWidth: true

                    TabButton {
                        id: clusterBtn

                        text: qsTr("Kafka Cluster")
                        width: implicitWidth
                    }

                }

                StackLayout {
                    id: stack

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: bar.currentIndex

                    Item {
                        id: kafkaCluster

                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        GridLayout {
                            anchors.fill: kafkaCluster
                            columns: 2

                            Text {
                                text: qsTr("Cluster Name")
                                color: Style.LabelColor
                                Layout.maximumWidth: 100
                            }

                            TextField {
                                id: clusterName

                                placeholderText: qsTr("Enter Cluster Name")
                                persistentSelection: true
                                selectByMouse: true
                                Layout.fillWidth: true
                            }

                            Text {
                                text: qsTr("Bootstrap Servers")
                                color: Style.LabelColor
                                Layout.maximumWidth: 100
                            }

                            TextField {
                                id: bootstrapServers

                                placeholderText: qsTr("eg: localhost:9092")
                                Layout.fillWidth: true
                                persistentSelection: true
                                selectByMouse: true
                            }

                            Text {
                                text: qsTr("Additional Properties")
                                color: Style.LabelColor
                                Layout.maximumWidth: 100
                            }

                            Flickable {
                                Layout.preferredHeight: 150
                                Layout.maximumHeight: 150
                                Layout.fillWidth: true

                                TextArea.flickable: TextArea {
                                    id: addProperties

                                    placeholderText: dialog.propertiesPlaceholder
                                    persistentSelection: true
                                    selectByMouse: true

                                    background: Rectangle {
                                        implicitHeight: 150
                                        implicitWidth: 500
                                        border.color: "#ababab"
                                    }

                                }

                                ScrollBar.vertical: ScrollBar {
                                }

                                ScrollBar.horizontal: ScrollBar {
                                }

                            }

                            Text {
                                text: qsTr("Color")
                                color: Style.LabelColor
                            }

                            ComboBox {
                                id: colorBox

                                editable: false
                                model: ["None", "Blue", "Red", "Green", "Yellow", "Pink", "Teal"]
                            }

                            Button {
                                Layout.column: 1
                                Layout.row: 4
                                text: qsTr("Test Kafka Connectivity")
                                onClicked: {
                                    if (bootstrapServers.text.length !== 0) {
                                        page.state = "testing";
                                        tester.test(bootstrapServers.text, addProperties.text);
                                    }
                                }

                                ConnectivityTester {
                                    id: tester

                                    onIsTested: (result) => {
                                        page.state = "default";
                                        testConnectDialog.open();
                                        staclLayout.currentIndex = result ? 0 : 1;
                                    }
                                }

                            }

                            Item {
                                id: filler

                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                        }

                    }

                }

            }

            BusyIndicator {
                id: busy

                anchors.centerIn: parent
                running: false
            }

        }

        Dialog {
            id: testConnectDialog

            modal: true
            standardButtons: Dialog.Close
            anchors.centerIn: Overlay.overlay
            width: 400
            height: 150

            StackLayout {
                id: staclLayout

                anchors.fill: parent
                currentIndex: 0

                Text {
                    text: qsTr("Connection to ") + bootstrapServers.text + qsTr(" OK!")
                }

                Text {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    wrapMode: Text.WordWrap
                    text: qsTr("It looks like [") + bootstrapServers.text + qsTr("] is not even accessible, check if the broker is up and its port opened.")
                }

            }

        }

        header: RowLayout {
            Text {
                text: "Cluster Configuration"
                color: "#484848"
                font.pointSize: 24
                Layout.leftMargin: 20
                Layout.topMargin: 15
            }

        }

        states: [
            State {
                name: "default"

                PropertyChanges {
                    target: page
                    enabled: true
                }

                PropertyChanges {
                    target: busy
                    running: false
                }

            },
            State {
                name: "testing"

                PropertyChanges {
                    target: page
                    enabled: false
                }

                PropertyChanges {
                    target: busy
                    running: true
                }

            }
        ]

        footer: RowLayout {
            Item {
                Layout.fillWidth: true
            }

            Button {
                Layout.bottomMargin: 10
                text: "Cancel"
                onClicked: dialog.clearAndClose()
            }

            Button {
                Layout.rightMargin: 10
                Layout.bottomMargin: 10
                enabled: clusterName.text.length !== 0 && bootstrapServers.text.length !== 0
                text: "Save"
                onClicked: {
                    dialog.createBroker(clusterName.text, bootstrapServers.text, Style.BrokerColor[colorBox.currentIndex], addProperties.text);
                    dialog.clearAndClose();
                }
            }

        }

    }

}
