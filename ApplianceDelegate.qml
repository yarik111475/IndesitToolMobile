import QtQuick 2.0
import QtQuick.Controls 2.5

Item{
    property var view: ListView.view;
    width: view.width;
    height: 80;

    Rectangle {
        anchors.fill: parent;
        radius: 5;
        smooth: true;
        color: "lightsteelblue";

        Column{
            anchors.fill: parent;
            anchors.margins: 10;
            spacing: 5;

            Text{
                text: model.name
                font.pointSize: 15;
                renderType: Text.NativeRendering;
            }
            Text{
                text: model.value;
                font.pointSize: 15;
                renderType: Text.NativeRendering;
                onTextChanged: {
                    if(model.name===qsTr("Checksum")){
                        color=model.value==="Valid" ? "green" : "red";
                    }
                }
            }
        }
    }
}


