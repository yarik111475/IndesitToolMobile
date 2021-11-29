import QtQuick 2.0
import QtQuick.Controls 2.12

Popup{
    id: aboutPopup;
    modal: true;
    bottomInset: -5;
    topInset: -5;
    leftInset: -5;
    rightInset: -5;
    visible: false;
    property alias applicationName: applicationNameText.text;
    property alias applicationVersion: applicationVersionText.text;

    onVisibleChanged: {
        if(visible){
            popupTimer.running=true;
        }
        else{
            popupTimer.running=false;
        }
    }

    onAboutToShow: {

        aboutAnimation.running=true;
    }

    onAboutToHide: {
        aboutAnimation.running=false;
    }

    PropertyAnimation{
        id: aboutAnimation;
        target: aboutPopup;
        properties: "opacity";
        from: 0;
        to: 1;
        duration: 300;
        running: false;
    }

    background: Rectangle{
        border{
            color: "black";
            width: 1;
        }
        radius: 5;
    }

    contentItem: Column{
        id: aboutColumn;
        anchors.centerIn: parent;
        Image{
            id: img;
            anchors.horizontalCenter: aboutColumn.horizontalCenter;
            width: 50;
            height: 50;
            fillMode: Image.PreserveAspectFit;
            source: "qrc:/icons/wash.png";
        }
        Text{
            id: applicationNameText;
            anchors.horizontalCenter: aboutColumn.horizontalCenter;
            horizontalAlignment: Text.AlignHCenter;
            font.pointSize: 15;
        }
        Text{
            id: applicationVersionText;
            anchors.horizontalCenter: aboutColumn.horizontalCenter;
            horizontalAlignment: Text.AlignHCenter;
            font.pointSize: 15;
        }
    }

    Timer{
        id: popupTimer;
        interval: 2000;
        repeat: false;
        running: false;
        onTriggered: {
            aboutPopup.visible=false;
        }
    }
}
