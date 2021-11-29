import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    id: saveFilePopup;
    modal: true;
    closePolicy: Popup.NoAutoClose;

    property alias text: fileNameTextInput.text;
    signal signalSave(string fileName, string extenshion);

    contentItem: Column{
        id: saveFileColumn;
        anchors.centerIn: parent;
        anchors.margins: 10;
        spacing: 10;

        Rectangle{
            id: saveFileRect;
            width: saveFileColumn.width;
            height: 25;
            anchors.margins: 10;
            border{
                color: "black";
                width: 1;
            }

            TextInput{
                id: fileNameTextInput;
                clip: true;
                anchors.centerIn: parent;
                anchors.margins: 5;
                horizontalAlignment: Text.AlignHCenter;


                readOnly: false;
                selectByMouse: true;
                font.pointSize: 15;
            }
        }

        Row{
            clip: true;
            width: saveFileColumn.width;
            anchors.margins: 10;
            anchors.horizontalCenter: saveFileColumn.horizontalCenter;
            spacing: 10;

            Button{
                id: saveButton
                text: qsTr("Save");
                icon.source: "qrc:/icons/select.png";
                font.pointSize: 15;
                onClicked: {
                    saveFilePopup.visible=false;
                    saveFilePopup.signalSave(fileNameTextInput.text,
                                    extensionComboBox.currentText);
                }
            }
            Button{
                id: cancelButton;
                text: qsTr("Cancel");
                icon.source: "qrc:/icons/cancel.png";
                font.pointSize: 15;
                onClicked: {
                    saveFilePopup.visible=false;
                }
            }
            ComboBox{
                id: extensionComboBox;
                font.pointSize: 15;
                model: ["*.eep", "*.bin"];
            }
        }
    }
}
