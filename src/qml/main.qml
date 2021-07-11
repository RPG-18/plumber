import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: appWindow

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
