import QtQuick 2.12
import QtQuick.Layouts 1.12
import "../style.js" as Style

Item {
    id: itm

    property int index: 0
    property string text: ""
    property string icon: ""

    signal clicked(int index)

    height: 56
    width: 200
    state: "default"

    Rectangle {
        id: bg

        anchors.fill: parent
        anchors.margins: 1
    }

    RowLayout {
        anchors.fill: parent
        spacing: 6

        Item {
            height: 56
            width: 56

            Image {
                anchors.centerIn: parent
                source: itm.icon
                sourceSize.width: 25
                sourceSize.height: 25
            }

        }

        Text {
            text: itm.text
            color: Style.LabelColor
            font.pixelSize: 24
            Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
        }

        Item {
            Layout.fillWidth: true
        }

    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            itm.state = itm.state === "default" ? "checked" : "default";
            itm.clicked(itm.index);
        }
    }

    states: [
        State {
            name: "default"

            PropertyChanges {
                target: bg
                color: "#ffffff"
            }

        },
        State {
            name: "checked"

            PropertyChanges {
                target: bg
                color: Style.LightGray
            }

        }
    ]
}
