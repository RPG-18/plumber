import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import plumber
import Qt.labs.platform as Platform
import "../style.js" as Style

Dialog {
    id: dialog

    standardButtons: Dialog.Ok | Dialog.Cancel

    property var exportHandler

    property url schemaFile: fileDialog.file

    onAccepted: {
        let error = dialog.exportHandler(exportFabric);
        if (err.isError) {
            consumerErrDialog.show(err);
        }
    }

    Platform.FileDialog {
        id: fileDialog
        fileMode: Platform.FileDialog.SaveFile
        nameFilters: csvButton.checked ? ["CSV files (*.csv)"] : ["JSON files (*.json)"]
    }

    ExportImportFabric {
        id: exportFabric
        file: fileDialog.file
        format: csvButton.checked ? 0 : 1 //todo refactoring to qml emun

        useTimestamp: timestampCb.checked
        usePartition: partitionCb.checked
        useOffset: offsetCb.checked
        useKey: keyCb.checked
        useValue: valueCb.checked
    }

    ButtonGroup {
        id: formatGroup
    }

    GridLayout {
        columns: 2

        Text {
            text: qsTr("Format")
        }

        RowLayout {
            RadioButton {
                id: csvButton
                text: "CSV"
                checked: true
                ButtonGroup.group: formatGroup
            }

            RadioButton {
                id: jsonButton

                text: "JSON"
                ButtonGroup.group: formatGroup
            }
        }

        Text {
            text: qsTr("Fields")
        }

        RowLayout {
            CheckBox {
                id: timestampCb
                checked: true
                text: qsTr("Timestamp")
            }

            CheckBox {
                id: partitionCb
                checked: true
                text: qsTr("Partition")
            }
            CheckBox {
                id: offsetCb
                checked: true
                text: qsTr("Offset")
            }
            CheckBox {
                id: keyCb
                checked: true
                text: qsTr("Key")
            }
            CheckBox {
                id: valueCb
                checked: true
                text: qsTr("Value")
            }
        }

        Text {
            text: "File"
        }

        RowLayout {
            Layout.fillWidth: true

            Text {
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
}
