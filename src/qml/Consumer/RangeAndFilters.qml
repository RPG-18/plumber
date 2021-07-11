import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../style.js" as Style

Item {
    id: item

    property alias headerKeyFilter: headerKey.text
    property alias headerValueFilter: headerValue.text
    property alias keyFilter: key.text
    property alias valueFilter: value.text
    property alias selectedFilter: filterSelector.selectedFilterId
    property alias startFromTimeBase: startFrom.selectedTimeBase
    property alias specificDateText: startFrom.specificDateText
    property alias selectedLimit: limit.selectedLimit
    property alias numberOfRecords: limit.numberOfRecords
    property alias maxSize: limit.maxSize
    property alias limitSpecificDateText: limit.specificDateText

    function resize() {
        simpleFilterValues.visible = contentGrid.visible && (filterSelector.selectedFilterId > 0);
    }

    width: 300
    implicitWidth: label.width
    implicitHeight: content.height

    ColumnLayout {
        id: content

        Text {
            id: label

            text: contentGrid.visible ? qsTr("▼ RANGE & FILTERS") : qsTr("▶ RANGE & FILTERS")
            color: Style.LabelColor
            font.bold: true
            font.pointSize: 14

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    contentGrid.visible = !contentGrid.visible;
                    resize();
                }
            }

        }

        GridLayout {
            id: contentGrid

            Layout.fillWidth: true
            columns: 2

            Text {
                Layout.minimumWidth: 70
                text: qsTr("Start From")
                color: Style.LabelColor
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            }

            StartInput {
                id: startFrom

                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.minimumWidth: 80
                onSelectedTimeBaseChanged: resize()
            }

            Text {
                Layout.minimumWidth: 70
                text: qsTr("Limit")
                color: Style.LabelColor
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            }

            LimitInput {
                id: limit

                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.minimumWidth: 80
            }

            Text {
                Layout.minimumWidth: 70
                text: qsTr("Filter")
                color: Style.LabelColor
                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            }

            FilterInput {
                id: filterSelector

                Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                Layout.minimumWidth: 80
                onSelectedFilterIdChanged: resize()
            }

        }

        Grid {
            id: simpleFilterValues

            columns: 2
            visible: false
            spacing: 6

            Text {
                text: "Key?"
            }

            TextField {
                id: key

                placeholderText: "A key to match. Optional"
            }

            Text {
                text: "Value?"
            }

            TextField {
                id: value

                placeholderText: "A value to match. Optional"
            }

            Text {
                text: "Header?"
            }

            Row {
                spacing: 6

                TextField {
                    id: headerKey

                    placeholderText: "Key. Optional"
                }

                TextField {
                    id: headerValue

                    placeholderText: "Value. Optional"
                }

            }

        }

    }

}
