import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import FluentUI 1.0

FluLauncher {
    id: app
    Component.onCompleted: {
        FluApp.init(app)
        FluRouter.routes = {
            "/":"qrc:/Drmix/main.qml",
        }
        FluRouter.navigate("/")
    }
}
