import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../style.js" as Style

Item {
    property alias keyTypeId: keyCbx.typeId
    property alias valueTypeId: valueCbx.typeId

    width: 300
    implicitHeight: content.height
    implicitWidth: content.width

    ColumnLayout {
        id: content

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
