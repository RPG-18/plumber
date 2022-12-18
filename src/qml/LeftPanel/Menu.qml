import QtQuick 2.12
import QtQuick.Layouts 1.12

Item {
    id: menu

    signal activatedItem(int indx)

    function setActiveItem(indx) {
        for (var i = 0; i < layout.children.length; i++) {
            if (i !== indx)
                layout.children[i].state = "default";
            else
                layout.children[i].state = "checked";
        }
    }

    function onClickHandler(indx) {
        for (var i = 0; i < layout.children.length; i++) {
            if (i !== indx)
                layout.children[i].state = "default";
        }
        menu.activatedItem(indx);
    }

    clip: true

    ColumnLayout {
        id: layout

        anchors.fill: parent
        spacing: 0

        MenuItem {
            index: 0
            text: qsTr("Overview")
            icon: "qrc:/home.svg"
            Layout.fillWidth: true
            state: "checked"
            onClicked: index => {
                menu.onClickHandler(index);
            }
        }

        MenuItem {
            index: 1
            text: qsTr("Brokers")
            icon: "qrc:/brokers.svg"
            Layout.fillWidth: true
            onClicked: index => {
                menu.onClickHandler(index);
            }
        }

        MenuItem {
            index: 2
            text: qsTr("Topics")
            icon: "qrc:/topics.svg"
            Layout.fillWidth: true
            onClicked: index => {
                menu.onClickHandler(index);
            }
        }

        MenuItem {
            index: 3
            text: qsTr("Consumers")
            icon: "qrc:/consumers.svg"
            Layout.fillWidth: true
            onClicked: index => {
                menu.onClickHandler(index);
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
