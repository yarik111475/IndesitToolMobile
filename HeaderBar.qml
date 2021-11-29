import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Extras 1.4;

ToolBar {
    id: root;
    property bool status: false;
    property int fontSize: 15;

    signal signalScan();
    signal signalRead();
    signal signalWrite();
    signal signalOpen();
    signal signalSave();
    signal signalSaveToDb();
    signal signalSettings();
    signal signalAbout();
    signal signalQuit();

    Rectangle{
        id: headerRect;
        width: root.width;
        height: 40;
        color: "lightsteelblue";

        Text{
            anchors.centerIn: parent;
            renderType: Text.NativeRendering;
            font.pointSize: 20;
            text: qsTr("Service Tool Mobile");
        }

        Rectangle{
            id: buttonRect;
            width: headerRect.height;
            height: width;
            color: parent.color;

            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;

            Image{
                width: parent.width;
                height: parent.height;
                anchors.centerIn: parent;
                fillMode: Image.PreserveAspectFit;
                source: "qrc:/icons/menu.png";
            }

            MouseArea{
                anchors.fill: parent;
                onClicked: {
                    menu.visible=true;
                }
                onPressed: {
                    buttonRect.color="steelblue";
                }
                onReleased: {
                    buttonRect.color=headerRect.color;
                }
            }
        }

        Rectangle{
            id: statusRect;

            width: headerRect.height;
            height: width;
            color: parent.color;

            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
            anchors.right: parent.right;

            StatusIndicator{
                anchors.centerIn: parent;
                width: parent.width/2;
                height: width;
                active: true;
                color: root.status ? "green" : "red";
            }
        }
    }

    Menu{
        id: menu;
        x: 0;
        y: parent.height;
        visible: false;

        onAboutToShow: {
            animation.running=true;
        }
        MenuItem{
            icon.source:  "qrc:/icons/scan.png";
            font.pointSize: fontSize;
            text: qsTr("Scan");
            onTriggered: {
                menuItemTriggered(0);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/read.png";
            font.pointSize: fontSize;
            text: qsTr("Read");
            onTriggered: {
                menuItemTriggered(1);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/write.png";
            font.pointSize: fontSize;
            text: qsTr("Write");
            onTriggered: {
                menuItemTriggered(2);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/open.png";
            font.pointSize: fontSize;
            text: qsTr("Open");
            onTriggered: {
                menuItemTriggered(3);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/save.png";
            font.pointSize: fontSize;
            text: qsTr("Save");
            onTriggered: {
                menuItemTriggered(4);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/add_to_base.png";
            font.pointSize: fontSize;
            text: qsTr("Add");
            onTriggered: {
                menuItemTriggered(5);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/settings.png";
            font.pointSize: fontSize;
            text: qsTr("Settings");
            onTriggered: {
                menuItemTriggered(6);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/about.png";
            font.pointSize: fontSize;
            text: qsTr("About");
            onTriggered: {
                menuItemTriggered(7);
            }
        }
        MenuItem{
            icon.source:  "qrc:/icons/quit.png";
            font.pointSize: fontSize;
            text: qsTr("Quit");
            onTriggered: {
                menuItemTriggered(8);
            }
        }


        PropertyAnimation{
            id: animation;
            target: menu;
            properties: "opacity";
            from: 0;
            to: 1;
            running: false;
            duration: 300;
        }
    }

    function menuItemTriggered(index){
        menu.visible=false;
        switch(index){
        case 0:
            root.signalScan();
            break;
        case 1:
            root.signalRead();
            break;
        case 2:
            root.signalWrite();
            break;
        case 3:
            root.signalOpen();
            break;
        case 4:
            root.signalSave();
            break;
        case 5:
            root.signalSaveToDb();
            break;
        case 6:
            root.signalSettings();
            break;
        case 7:
            root.signalAbout();
            break;
        case 8:
            root.signalQuit();
            break;
        }
    }
}

