import QtQuick 2.12
import QtQuick.Controls 2.12
import "../style.js" as Style

Dialog {
    id: dialog

    function show(e) {
        label.text = e.what;
        dialog.open();
    }

    title: qsTr("Error")
    modal: true
    standardButtons: Dialog.Ok

    Text {
        id: label

        anchors.fill: parent
        wrapMode: Text.WordWrap
        font.family: Style.FontFamily
    }

}
