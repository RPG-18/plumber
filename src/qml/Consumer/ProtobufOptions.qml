import QtQuick 6.0
import QtQuick.Layouts 6.0
import QtQuick.Controls 6.0
import Qt.labs.platform 1.0
import plumber 1.0
import "../style.js" as Style

Item {
    id: itm
    width: 250
    implicitHeight: content.height

    property url protoFile: fileDialog.file
    property alias protoMessage: messageCbx.currentText

    ColumnLayout {
        id: content
        width: parent.width
        spacing: 4

        RowLayout {
            spacing: 4
            Layout.fillWidth: true

            Text {
                text: "Proto:"
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
        ComboBox {
            id: messageCbx
            Layout.fillWidth: true
            model: protoMessages
            textRole: "message"
        }
    }
    FileDialog {
        id: fileDialog
        nameFilters: ["Proto files (*.proto)"]
        onFileChanged: {
            itm.protoFile = file;
        }
    }
    ProtobufMessagesModel {
        id: protoMessages
        file: fileDialog.file
        onLoaded: {
            messageCbx.currentIndex = 0;
        }
        onParseErrors: errs => {
            let e = {
                "what": errs
            };
            consumerErrDialog.show(e);
        }
    }
}
