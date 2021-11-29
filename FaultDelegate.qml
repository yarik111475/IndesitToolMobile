import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    signal signalViewDescription(string description);
    property var view: ListView.view;
    property string faultDescription: model.clear ? qsTr("Fault not presents") : qsTr("Click to view");
    width: view.width;
    height: 125;
    radius: 5;
    color: model.clear ? "lightgreen" : "red";

    Column{
        anchors.fill: parent;
        anchors.margins: 10;
        spacing: 5;

        Text{
            text: qsTr("Code: ") + model.code;
            font.pointSize: 15;
            renderType: Text.NativeRendering;
        }
        Text{
            text: qsTr("Subcode: ") + model.subcode;
            font.pointSize: 15;
            renderType: Text.NativeRendering;
        }
        Text{
            text: qsTr("Occurence: ") + model.occurence;
            font.pointSize: 15;
            renderType: Text.NativeRendering;
        }
        Text{
            text: qsTr("Description: ") + faultDescription;
            font.pointSize: 15;
            renderType: Text.NativeRendering;
            onTextChanged: {
                if(!model.clear){
                    font.underline=true;
                    color="white";
                }
            }
        }
    }

    MouseArea{
        anchors.fill: parent;
        onClicked: {
            if(!model.clear){
                parent.signalViewDescription(model.description);
            }
        }
    }
}
