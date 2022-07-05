import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import plumber
import "../style.js" as Style

Item {
    id: itm

    property url schemaFile: fileDialog.file

    width: 250
    implicitHeight: content.height

    ColumnLayout {
        id: content

        width: parent.width
        spacing: 4

        RowLayout {
            spacing: 4
            Layout.fillWidth: true

            Text {
                text: "Avro Schema:"
                color: Style.LabelColor
            }

            Text {
                id: path

                Layout.fillWidth: true
                elide: Text.ElideLeft
                horizontalAlignment: Text.AlignRight
                text: fileDialog.file
                color: Style.LabelColor
            }

            Button {
                text: "..."
                width: 38
                implicitWidth: 38
                onClicked: fileDialog.open()
            }
        }
    }

    FileDialog {
        id: fileDialog

        nameFilters: ["Avro schema files (*.avsc)", "JSON Schema files (*.json)"]
        onFileChanged: {
            itm.schemaFile = file;
        }
    }
}
