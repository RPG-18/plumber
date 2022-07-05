import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: appWindow

    title: "Plumber"
    width: 700
    height: 400
    visible: true

    StackView {
        id: stack

        initialItem: startPage
        anchors.fill: parent

        pushEnter: Transition {
        }

        pushExit: Transition {
        }
    }

    Component {
        id: startPage

        StartScreen {
        }
    }

    Component {
        id: mainPage

        MainScreen {
        }
    }
}
