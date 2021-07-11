import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../style.js" as Style

Item {
    property alias keyTypeId: keyCbx.typeId
    property alias valueTypeId: valueCbx.typeId

    width: 300
    implicitHeight: contentGrid.visible ? label.height + contentGrid.height : label.height
    implicitWidth: label.width

    ColumnLayout {
        id: content

        anchors.fill: parent

        Text {
            id: label

            text: contentGrid.visible ? qsTr("▼ FORMAT") : qsTr("▶ FORMAT")
            color: Style.LabelColor
            font.bold: true
            font.pointSize: 14

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    contentGrid.visible = !contentGrid.visible;
                }
            }

        }

        GridLayout {
            id: contentGrid

            columns: 2

            Text {
                Layout.minimumWidth: 70
                text: qsTr("Key")
                color: Style.LabelColor
            }

            TypeCombobox {
                id: keyCbx
            }

            Text {
                Layout.minimumWidth: 70
                text: qsTr("Value")
                color: Style.LabelColor
            }

            TypeCombobox {
                id: valueCbx
            }

        }

    }

}
