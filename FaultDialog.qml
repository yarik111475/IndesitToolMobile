import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.5

Popup {
    id: root;
    modal: true;
    bottomInset: -5;
    topInset: -5;
    leftInset: -5;
    rightInset: -5;
    closePolicy: Popup.CloseOnPressOutside;
    contentHeight: view.implicitHeight;
    contentWidth: view.implicitWidth;

    property alias text: txt.text;

    background: Rectangle{
        border{
            color: "black";
            width: 1;
        }
        radius: 5;
    }

    contentItem: ScrollView{
        id: view;
        anchors.fill: parent;
        TextArea{
            id: txt;
            readOnly: true;
            font.pointSize: 15;
            renderType: TextArea.NativeRendering;
            wrapMode: TextArea.Wrap;
        }
    }
}
