import QtQuick 2.9
import QtQuick.Controls 2.5

Popup {
    id: root;
    modal: true;
    topInset: -5;
    bottomInset: -5;
    leftInset: -5;
    rightInset: -5;
    closePolicy: Popup.CloseOnPressOutside;

    property string applianceName;
    signal signalSelect(string selectedFileName );

    onAboutToShow: {
        openFileDialogAnimation.running=true;
        if(listModel.rowCount()!==0){
            listModel.remove(0, listModel.rowCount());
        }

        var list=fileHandler.getCurrentFilesList(applianceName, "*.eep");
        if(list.length!==0){
            for(var i=0; i<list.length; i++){
                listElement.name=list[i].Model;
                listElement.code=list[i].Code;
                listElement.serial=list[i].Serial;

                listModel.append(listElement);
            }
        }
    }

    onAboutToHide: {
        openFileDialogAnimation.running=false;
    }

    PropertyAnimation{
        id: openFileDialogAnimation;
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

    contentItem: ListView{
        id: view;
        anchors.fill: parent;
        anchors.margins: 5;
        spacing: 10;
        clip: true;

        header: Rectangle{
            width: view.width;
            height: 40;
            radius: 5;

            Text{
                anchors.centerIn: parent;
                font.pointSize: 20;
                renderType: Text.NativeRendering;
                text: qsTr("Current files");
            }
        }

        model: listModel;

        delegate: Item{
            property var view: ListView.view;
            width: view.width;
            height: 100;

            Rectangle{
                anchors.fill: parent;
                color: "lightsteelblue";
                radius: 5;

                Column{
                    anchors.fill: parent;
                    anchors.margins: 10;
                    spacing: 5;

                    Text{
                        text: model.name;
                        font.pointSize: 15;
                        renderType: Text.NativeRendering;
                    }
                    Text{
                        text: model.code;
                        font.pointSize: 15;
                        renderType: Text.NativeRendering;
                    }
                    Text{
                        text: model.serial;
                        font.pointSize: 15;
                        renderType: Text.NativeRendering;
                    }
                }
                MouseArea{
                    anchors.fill: parent;
                    onClicked: {
                        var fileName=model.name + "_" + model.code + "_" + model.serial + ".eep";
                        root.signalSelect(fileName);
                        root.close();
                    }
                }
            }
        }
    }

    ListModel{
        id: listModel;
    }

    ListElement{
        id: listElement;
        property string name;
        property string code;
        property string serial;
    }
}
