import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import plumber
import "../style.js" as Style

Item {
    id: itm

    property url protoFile: fileDialog.file
    property alias protoMessage: messageCbx.currentText

    implicitHeight: content.height
    width: 500

    RowLayout {
        id: content

        width: parent.width
        spacing: 4

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

        ComboBox {
            id: messageCbx

            width: 170
            Layout.preferredWidth: 170
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
        onParseErrors: (errs) => {
            let e = {
                "what": errs
            };
            consumerErrDialog.show(e);
        }
        onLoaded: {
            messageCbx.currentIndex = 0;
        }
    }

}
