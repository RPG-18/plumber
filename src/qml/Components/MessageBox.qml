import QtQuick 2.12
import QtQuick.Controls 2.12
import "../style.js" as Style

Dialog {
    id: dialog
    property alias text: label.text

    property var accaptHanlder: null
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
        if (accaptHanlder !== null)
            accaptHanlder();
    }

    Text {
        id: label
        anchors.fill: parent
        wrapMode: Text.WordWrap
        font.family: Style.FontFamily
    }
}
