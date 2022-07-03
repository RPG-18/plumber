import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import plumber 1.0
import "../style.js" as Style
import "../Components"

Item {
    property alias keyTypeId: keyCbx.typeId
    property alias valueTypeId: valueCbx.typeId
    property alias keyProto: protoKeyId
    property alias valueProto: protoValueId
    property alias keyAvro: avroKeyId
    property alias valueAvro: avroValueId

    width: 320
    implicitHeight: content.height
    implicitWidth: content.width

    ProtoOption {
        id: protoKeyId

        file: protobufOptKey.protoFile
        message: protobufOptKey.protoMessage
    }

    ProtoOption {
        id: protoValueId

        file: protobufOptValue.protoFile
        message: protobufOptValue.protoMessage
    }

    AvroOption {
        id: avroKeyId

        file: avroOptKey.schemaFile
    }

    AvroOption {
        id: avroValueId

        file: avroOptValue.schemaFile
    }

    ColumnLayout {
        id: content

        Text {
            id: label

            text: contentGrid.visible ? qsTr("▼ FORMAT") : qsTr("▶ FORMAT")
            color: Style.LabelColor
            font.bold: true
            font.pointSize: 14

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    contentGrid.visible = !contentGrid.visible;
                }
            }
        }

        ColumnLayout {
            id: contentGrid

            GridLayout {
                columns: 2

                Text {
                    Layout.minimumWidth: 70
                    text: qsTr("Key")
                    color: Style.LabelColor
                }

                TypeCombobox {
                    id: keyCbx
                }
            }

            ProtobufOptions {
                id: protobufOptKey

                visible: keyCbx.typeId == 7
                Layout.columnSpan: 2
                Layout.preferredWidth: 320
            }

            AvroOptions {
                id: avroOptKey

                visible: keyCbx.typeId == 8
                Layout.columnSpan: 2
                Layout.preferredWidth: 320
            }

            GridLayout {
                columns: 2

                Text {
                    Layout.minimumWidth: 70
                    text: qsTr("Value")
                    color: Style.LabelColor
                }

                TypeCombobox {
                    id: valueCbx
                }
            }

            ProtobufOptions {
                id: protobufOptValue

                visible: valueCbx.typeId == 7
                Layout.columnSpan: 2
                Layout.preferredWidth: 320
            }

            AvroOptions {
                id: avroOptValue

                visible: valueCbx.typeId == 8
                Layout.columnSpan: 2
                Layout.preferredWidth: 320
            }
        }
    }
}
