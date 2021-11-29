import QtQuick 2.0
import QtQuick.Controls 2.5

Popup{
    id: root;
    modal: true;
    topInset: -5;
    bottomInset: -5;
    leftInset: -5;
    rightInset: -5;
    closePolicy: Popup.CloseOnPressOutside
    property alias text: messageText.text;
    property alias source: messageImage.source;

    onAboutToShow: {
        messageDialogAnimation.running=true;
    }
    onAboutToHide: {
        messageDialogAnimation.running=false;
    }

    PropertyAnimation{
        id: messageDialogAnimation;
        properties: "opacity";
        target: root;
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



    contentItem: Rectangle{
        id: messageRectangle;

        Column{
            id: messageColumn;
            anchors.fill: parent;
            Image{
                id: messageImage;
                width: 50;
                height: 50;
                fillMode: Image.PreserveAspectFit;
                anchors.horizontalCenter: messageColumn.horizontalCenter;
            }
            Text{
                id: messageText;
                anchors.horizontalCenter: messageColumn.horizontalCenter;
                horizontalAlignment: Text.AlignHCenter;
                font.pointSize: 15;
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere;
            }
        }
    }
}

