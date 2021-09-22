import QtQuick 6.0
import QtQuick.Controls 6
import QtQuick.Layouts 6.0
import kafkaui 1.0
import "../Components" as Components

Item {
    property var topicModel
    property alias selectedTopic: topicBox.currentValue
    property alias keyTypeId: keyCbx.typeId
    property alias valueTypeId: valueCbx.typeId

    function keyData() {
        if (keyCbx.typeId === 6)
            return "";

        if (keyCbx.typeId !== 0)
            return keyTxt.text;

        return keyArea.text;
    }

    function valueData() {
        if (valueCbx.typeId === 6)
            return "";

        if (valueCbx.typeId !== 0)
            return valueTxt.text;

        return valueArea.text;
    }

    height: 100
    width: 100

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8
        anchors.bottomMargin: 0
        columns: 2

        Text {
            text: qsTr("Topic:")
        }

        ComboBox {
            id: topicBox

            model: topicModel
            textRole: "topic"
            valueRole: "topic"
            Layout.preferredWidth: 300
        }

        Text {
            text: qsTr("Key:")
        }

        Components.TypeCombobox {
            id: keyCbx
        }

        TextField {
            id: keyTxt

            Layout.columnSpan: 2
            Layout.fillWidth: true
            selectByMouse: true
            visible: keyCbx.typeId >= 1 && keyCbx.typeId < 6
        }

        Components.TextArea {
            id: keyArea

            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            visible: keyCbx.typeId === 0
        }

        Rectangle {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            height: 1
            color: "#f0f0f0"
        }

        Text {
            text: qsTr("Value:")
        }

        Components.TypeCombobox {
            id: valueCbx
        }

        TextField {
            id: valueTxt

            Layout.columnSpan: 2
            Layout.fillWidth: true
            selectByMouse: true
            visible: valueCbx.typeId >= 1 && valueCbx.typeId < 6
        }

        Components.TextArea {
            id: valueArea

            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: valueCbx.typeId === 0
        }

        Item {
            visible: valueCbx.typeId !== 0
            Layout.columnSpan: 2
            Layout.fillHeight: true
        }

    }

}
