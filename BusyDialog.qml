import QtQuick 2.0
import QtQuick.Controls 2.5

Popup{
    id: root;
    modal: true;
    bottomInset: -5;
    topInset: -5;
    leftInset: -5;
    rightInset: -5;
    visible: false;
    closePolicy: Popup.NoAutoClose;

    property alias text: txt.text;
    property alias color: txt.color;   
    anchors.centerIn: parent;

    background: Rectangle{
        border{
            color: "black";
            width: 1;
        }
        radius: 5;
    }
    contentItem: Column{
        id: busyIndicatorColumn;
        anchors.centerIn: parent;
        BusyIndicator{
            height: root.height/2;
            width: height;
            anchors.horizontalCenter: parent.horizontalCenter;
        }
        Text{
            id: txt;
            font.pointSize: 20;
            renderType: Text.NativeRendering;
            anchors.horizontalCenter: parent.horizontalCenter;
        }
    }
}
