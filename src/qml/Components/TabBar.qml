import QtQuick
import QtQuick.Controls 6.0 as Controls

Controls.TabBar {
    id: control

    implicitHeight: 44

    background: Rectangle {
        color: "#ffffff"
    }

    contentItem: ListView {
        model: control.contentModel
        currentIndex: control.currentIndex
        spacing: 1
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapToItem
        highlightMoveDuration: 0
        highlightResizeDuration: 0
        highlightFollowsCurrentItem: true
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: 44
        preferredHighlightEnd: width - 44

        highlight: Item {
            z: 2

            Rectangle {
                height: 2
                width: parent.width
                y: parent.height - height
                color: "#5186f9"
            }

        }

    }

}
