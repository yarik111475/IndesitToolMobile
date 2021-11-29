import QtQuick 2.0
import QtQuick.Controls 2.5

Popup{
    id: progressPopup;
    modal: true;
    topInset: -5;
    bottomInset: -5;
    leftInset: -5;
    rightInset: -5;
    closePolicy: Popup.NoAutoClose;

    onAboutToShow: {
        animation.running=true;
    }
    onAboutToHide: {
        animation.running=false;
    }

    PropertyAnimation{
        id: animation;
        properties: "opacity";
        target: progressPopup;
        from: 0;
        to: 1;
        duration: 100;
        running: false;
    }

    property alias text: progressText.text;
    property alias color: progressText.color;

    property alias from: progressBar.from;
    property alias to: progressBar.to;
    property alias value: progressBar.value;
    property alias indeterminate: progressBar.indeterminate;

    background: Rectangle{
        border{
            color: "black";
            width: 1;
        }
        radius: 5;
    }

    contentItem: Column{
        id: progressColumn;
        anchors.centerIn: parent;
        anchors.margins: 10;
        spacing: 10;

        Text{
            id: progressText
            font.pointSize: 20;
            renderType: Text.NativeRendering;
            anchors.horizontalCenter: progressColumn.horizontalCenter;
            horizontalAlignment: Text.AlignHCenter;
        }
        ProgressBar{
            id: progressBar;
            width: progressColumn.width;
        }
    }
}
