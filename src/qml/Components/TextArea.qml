import QtQuick 6.0
import QtQuick.Controls 6.0

Flickable {
    id: control

    property alias internalArea: area
    property alias text: area.text

    TextArea.flickable: TextArea {
        id: area

        persistentSelection: true
        selectByMouse: true

        background: Rectangle {
            implicitHeight: control.width
            implicitWidth: 500
            border.color: "#ababab"
        }
    }
}
