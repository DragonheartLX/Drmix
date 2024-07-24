import QtQuick 2.15
import QtQuick.Window 2.15
import FluentUI 1.0

FluWindow {
    width: 640
    height: 480
    minimumWidth: 320
    minimumHeight: 240
    title: qsTr("Drmix")

    Column{
        anchors.centerIn: parent
        spacing: 15
        FluText{
            text: qsTr("Welcome to FluentUI")
            anchors.horizontalCenter: parent.horizontalCenter
            font: FluTextStyle.Title
        }
        FluFilledButton{
            text: qsTr("Learn FluentUI")
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                console.log("Test")
            }
        }
    }
}
