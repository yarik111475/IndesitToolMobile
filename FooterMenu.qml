import QtQuick 2.0
import QtQuick.Controls 2.5

ToolBar{
    id: root;
    height: 45;
    signal signalScan();
    signal signalConnect();
    signal signalRead();
    signal signalWrite();
    signal signalOpen();
    signal signalSave();
    signal signalSettings();
    signal signalQuit();

    Rectangle{
        anchors.fill: parent;
        color: "lightgray";
    }

    Rectangle{
        id: menuRectangle;
        height: root.height;
        width: height;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        color: "lightgray";
    }

    Image{
        id: img;
        width: menuRectangle.width;
        height: menuRectangle.height;
        anchors.centerIn: menuRectangle;
        source: "qrc:/icons/menu.png";
    }

    MouseArea{
        anchors.fill: menuRectangle;
        onClicked: {
            menu.x=menuRectangle.x;
            menu.y=menuRectangle.y;
            menu.open();
        }
        onPressed: {
            menuRectangle.color="gray";
        }
        onReleased: {
            menuRectangle.color="lightgray";
        }
    }

    Menu{
        id: menu;
        visible: false;
        width: 45;
        MenuItem{
            icon.source:"qrc:/icons/scan.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: root.signalScan();
        }
//        MenuItem{
//            icon.source:"qrc:/icons/connect.png";
//            icon.height: 50;
//            icon.width: 50;
//            onTriggered: root.signalConnect();
//        }
        MenuItem{
            icon.source:"qrc:/icons/read.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: {
                menu.close();
                root.signalRead();
            }
        }
        MenuItem{
            icon.source:"qrc:/icons/write.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: {
                menu.close();
                root.signalWrite();
            }
        }
        MenuItem{
            icon.source:"qrc:/icons/open.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: root.signalOpen();
        }
//        MenuItem{
//            icon.source:"qrc:/icons/save.png";
//            icon.height: 50;
//            icon.width: 50;
//            onTriggered: root.signalSave();
//        }
        MenuItem{
            icon.source:"qrc:/icons/settings.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: root.signalSettings();
        }
        MenuItem{
            icon.source:"qrc:/icons/quit.png";
            icon.height: 45;
            icon.width: 45;
            onTriggered: root.signalQuit();
        }
    }
}
