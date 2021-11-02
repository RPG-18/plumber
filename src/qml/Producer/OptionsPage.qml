import QtQuick 6.0
import QtQuick.Controls 6.0
import QtQuick.Layouts 6.0

Item {
    property alias compression: compressionGroup.selectedIdx
    property alias ack: ackGroup.selectedIdx
    property alias idempotence: idempotenceCb.checked

    height: 100
    width: 100

    ButtonGroup {
        id: compressionGroup

        property int selectedIdx: 0

        buttons: compressionLayout.children
        onClicked: (btn) => {
            for (var i = 0; i < compressionLayout.children.length; i++) {
                if (compressionLayout.children[i] === btn)
                    selectedIdx = i;

            }
        }
    }

    ButtonGroup {
        id: ackGroup

        property int selectedIdx: 2

        buttons: ackLayout.children
        onClicked: (btn) => {
            for (var i = 0; i < ackLayout.children.length; i++) {
                if (ackLayout.children[i] === btn)
                    selectedIdx = i;

            }
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8
        anchors.bottomMargin: 0
        columns: 2

        Text {
            text: qsTr("Compression Type")
        }

        RowLayout {
            id: compressionLayout

            Layout.fillWidth: true

            Button {
                checkable: true
                checked: true
                text: "none"
                Layout.maximumWidth: 50
            }

            Button {
                checkable: true
                text: "gzip"
                Layout.maximumWidth: 50
            }

            Button {
                checkable: true
                text: "snappy"
                Layout.maximumWidth: 75
            }

            Button {
                checkable: true
                text: "lz4"
                Layout.maximumWidth: 50
            }

            Button {
                checkable: true
                text: "zstd"
                Layout.maximumWidth: 50
            }

            Item {
                Layout.fillWidth: true
            }

        }

        Text {
            text: qsTr("Idempotence")
        }

        CheckBox {
            id: idempotenceCb
        }

        Text {
            text: qsTr("Ack")
        }

        RowLayout {
            id: ackLayout

            Layout.fillWidth: true

            Button {
                checkable: true
                text: "none"
                enabled: !idempotenceCb.checked
                Layout.maximumWidth: 50
            }

            Button {
                checkable: true
                text: "leader"
                enabled: !idempotenceCb.checked
                Layout.maximumWidth: 75
            }

            Button {
                checkable: true
                checked: true
                text: "all"
                enabled: !idempotenceCb.checked
                Layout.maximumWidth: 38
            }

            Item {
                Layout.fillWidth: true
            }

        }

        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
        }

    }

}
