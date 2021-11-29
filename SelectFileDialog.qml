import QtQuick 2.4
import QtQuick.Controls 2.5

Rectangle {
    Connections{
        target: fileHandler;
    }

    id: root;
    property string selectedFileName;
    signal signalCancel();
    signal signalSelect();

    onVisibleChanged: {
        if(visible){
            listView.model=fileHandler.getCurrentFilesList(extensionComboBox.currentText);
        }
    }
    border{
        color: "black";
        width: 1;
    }
    smooth: true;

    Column{
        id: listViewColumn;
        anchors.top: root.top;
        anchors.bottom: buttonsRow.top;
        anchors.left: root.left;
        anchors.right: root.right;
        anchors.margins: 10;
        spacing: 10;

        ListView{
            id: listView;
            width: listViewColumn.width;
            height: listViewColumn.height-20;

            highlight: Rectangle{
                color: "lightgray";
            }
            highlightFollowsCurrentItem: true;

            delegate: Item{
                id: elementItem;
                width: listViewColumn.width;
                height: 30;
                Text{
                    anchors.margins: 5;
                    anchors.verticalCenter: parent.verticalCenter;
                    horizontalAlignment: Text.AlignLeft;
                    text: modelData;
                    font.pointSize: 15;
                }
                MouseArea{
                    anchors.fill: parent;
                    onClicked: {
                        listView.currentIndex=model.index;
                    }
                }
            }
        }
    }

    Row{
        id: buttonsRow;
        anchors.bottom: root.bottom;
        anchors.horizontalCenter: root.horizontalCenter;
        anchors.margins: 10;
        spacing: 10;
        clip: true;

        Button{
            id: buttonSelect;
            text: qsTr("Select");
            font.pointSize: 20;
            icon.source: "qrc:/icons/select.png";
            onClicked: {
                root.visible=false;
                selectedFileName=listView.model[listView.currentIndex].toString();
                root.signalSelect();
            }
        }
        Button{
            id: buttonCancel;
            text: qsTr("Cancel");
            font.pointSize: 20;
            icon.source: "qrc:/icons/cancel.png";
            onClicked: {
                root.visible=false;
                root.signalCancel();
            }
        }
        ComboBox{
            id: extensionComboBox;
            model: ["eep", "bin"];
            font.pointSize: 15;
            onCurrentTextChanged: {
                listView.model=fileHandler.getCurrentFilesList(currentText);
            }
        }
    }
}
