import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2

//MNain application window
ApplicationWindow {
    id: mainWindow;
    visible: true
    width: 400
    height: 640
    MenuDialog{
        anchors.centerIn: parent;
        width: parent.width-20;
        height: width;
        visible: true;
    }
}
