import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import plumber
import "style.js" as Style
import "Components" as Components
import "Group" as Group

Item {
    id: item

    width: 300
    height: 150

    StackView {
        id: groupStack
        anchors.fill: parent
        initialItem: groupsView

        pushEnter: Transition {
        }

        pushExit: Transition {
        }
    }

    Component {
        id: groupsView
        Group.GroupsView {
            width: item.width
            height: item.height

            onSelectedGroup: group => {
                let obj = groupView.createObject(item, {
                        "group": group
                    });
                groupStack.push(obj);
            }

            consumerModel: mainCluster.consumerModel()
        }
    }

    Component {
        id: groupView

        Group.GroupView {
            width: item.width
            height: item.height
        }
    }
}
