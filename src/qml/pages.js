.import "constants.js" as Constants

function createMainScreen(broker) {
    largeWindow()
    stack.push(mainPage, {"broker": broker})
}

function smallWindow() {
    appWindow.width = 700
    appWindow.height = 400
}

function largeWindow() {
    appWindow.width = 1280
    appWindow.height = 720
}

function createConsumerScreen(topic, topicModel, broker) {
    let component = Qt.createComponent("qrc:/qml/Consumer/ConsumerScreen.qml");
    let posX = appWindow.x + appWindow.width/2 - Constants.ConsumerScreenWidth/2;
    let posY = appWindow.y + appWindow.height/2 - Constants.ConsumerScreenHeight/2;
    let state = {
        x: posX,
        y: posY,
        topic:topic,
        topicModel: topicModel,
        broker: broker
    };

    let consumer = component.createObject(appWindow, state);
}

function createMessageScreen(x,y,width,height,  message) {
    let component = Qt.createComponent("qrc:/qml/Consumer/MessageView.qml");
    let posX = x + width/2 - Constants.MessageViewWidth/2;
    let posY = y + height/2 - Constants.MessageViewHeight/2;
    let state = {
        x: posX,
        y: posY,
        message:message,
    };

    let consumer = component.createObject(appWindow, state);
}

function createTopicCreateScreen(broker) {
    let component = Qt.createComponent("qrc:/qml/CreateTopicDialog.qml");
    let posX = appWindow.x + appWindow.width/2 - Constants.ConsumerScreenWidth/2;
    let posY = appWindow.y + appWindow.height/2 - Constants.ConsumerScreenHeight/2;

    let state = {
        x: posX,
        y: posY,
        broker:broker,
    };

    let dialog = component.createObject(appWindow, state);
}
