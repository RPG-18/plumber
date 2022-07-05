import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../style.js" as Style

Rectangle {
    id: menu

    property var broker
    property var settings

    signal activatedItem(int indx)

    function setActiveItem(indx) {
        leftMenu.setActiveItem(indx);
    }

    width: 300
    height: 200
    radius: 10
    border.color: Style.BorderColor
    state: settings.leftPanelState

    Rectangle {
        id: header

        width: parent.width
        height: 80
        anchors.top: parent.top
        radius: parent.radius
        border.color: Style.BorderColor
        state: "default"

        RowLayout {
            anchors.fill: parent
            spacing: 4

            Image {
                id: kafka_icon

                source: "qrc:/kafka_icon.svg"
                height: 50
                width: 50
                Layout.alignment: Qt.AlignVCenter
            }

            Text {
                id: headerLabel

                text: menu.broker.name
                color: menu.broker.color
                font.pixelSize: 35
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
            }

            Item {
                Layout.fillWidth: true
            }
        }

        states: [
            State {
                name: "default"

                PropertyChanges {
                    target: header
                    color: "white"
                }

                PropertyChanges {
                    target: headerLabel
                    visible: true
                }
            },
            State {
                name: "small"

                PropertyChanges {
                    target: header
                    color: menu.broker.color
                }

                PropertyChanges {
                    target: headerLabel
                    visible: false
                }
            }
        ]
    }

    Rectangle {
        width: parent.width
        height: 88
        border.color: Style.BorderColor
        anchors.top: header.bottom
        anchors.topMargin: -6
        anchors.bottom: collapsBtn.top
        anchors.bottomMargin: -4

        Menu {
            id: leftMenu

            anchors.fill: parent
            onActivatedItem: indx => {
                menu.activatedItem(indx);
            }
        }
    }

    Rectangle {
        id: collapsBtn

        anchors.bottom: parent.bottom
        width: parent.width
        height: 32
        radius: parent.radius
        border.color: Style.BorderColor

        Text {
            id: collapsBtnLabel

            anchors.centerIn: parent
            text: qsTr("<< Collapse")
            color: Style.LabelColor
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (menu.state === "default")
                    settings.leftPanelState = "small";
                else
                    settings.leftPanelState = "default";
            }
        }
    }

    states: [
        State {
            name: "default"

            PropertyChanges {
                target: collapsBtnLabel
                text: qsTr("« Collapse")
            }

            PropertyChanges {
                target: menu
                width: 300
                implicitWidth: 300
            }

            PropertyChanges {
                target: header
                state: "default"
            }

            PropertyChanges {
                target: kafka_icon
                source: "qrc:/kafka_icon.svg"
            }
        },
        State {
            name: "small"

            PropertyChanges {
                target: collapsBtnLabel
                text: "»"
            }

            PropertyChanges {
                target: menu
                width: 60
                implicitWidth: 60
            }

            PropertyChanges {
                target: header
                state: "small"
            }

            PropertyChanges {
                target: kafka_icon
                source: menu.broker.color !== Style.BrokerColor[0] ? "qrc:/kafka_icon_reverse.svg" : "qrc:/kafka_icon.svg"
            }
        }
    ]
}
