import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    ListModel{
        id: dataModel;
        ListElement{
            value: "Russian";
        }
        ListElement{
            value: "English";
        }
    }
    id: settingsPopup;
    modal: true;
    visible: false;
    topInset: -5;
    bottomInset: -5;
    leftInset: -5;
    rightInset: -5;
    closePolicy: Popup.CloseOnPressOutside;

    onAboutToShow: {
        settingsAnimation.running=true;
    }

    onAboutToHide: {
        settingsAnimation.running=false;
    }

    PropertyAnimation{
        id: settingsAnimation;
        target: settingsDialog;
        properties: "opacity";
        from: 0;
        to: 1;
        duration: 300;
        running: false;
    }

    signal signalRescanClick();
    signal signalConnectClick();
    signal signalOkClick(string applainceName);
    signal signalApplianceNameChanged(string applianceName);

    property string languageName;
    property string applianceName;
    property string bluetoothName;

    property alias bluetoothModel: bluetoothComboBox.model;

    background: Rectangle{
        border{
            color: "black";
            width: 1;
        }
        radius: 5;
    }

    contentItem: Column{
        id: settingsColumn;
        anchors.centerIn: parent;
        anchors.margins: 10;
        spacing: 10;

        Text{
            width: settingsColumn.width;
            font.pointSize: 15;
            text: qsTr("Select appliance");
            horizontalAlignment: Text.AlignHCenter
        }
        ComboBox{
            id: appliancesComboBox;
            width: settingsColumn.width;
            font.pointSize: 15;
            model:["ARCADIA", "EVO_1", "EVO_2"];
            onCurrentTextChanged: applianceName=currentText;

        }
        Text{
            width: settingsColumn.width;
            font.pointSize: 15;
            text: qsTr("Select bluetooth");
            horizontalAlignment: Text.AlignHCenter
        }
        ComboBox{
            id: bluetoothComboBox;
            width: settingsColumn.width;
            font.pointSize: 15;
            onCurrentTextChanged: bluetoothName=currentText;
        }
        RoundButton{
            text: qsTr("Rescan");
            width: settingsColumn.width;
            font.pointSize: 15;
            icon.source: "qrc:/icons/scan.png";
            onClicked: {
                settingsPopup.visible=false;
                settingsPopup.signalRescanClick();
            }
        }
        RoundButton{
            text: qsTr("Connect");
            width: settingsColumn.width;
            font.pointSize: 15;
            icon.source: "qrc:/icons/connect.png";
            onClicked: {
                settingsPopup.visible=false;
                settingsPopup.signalConnectClick();
            }
        }
        RoundButton{
            text: qsTr("Ok");
            width: settingsColumn.width;
            font.pointSize: 15;
            icon.source: "qrc:/icons/ok.png";
            onClicked: {
                settingsPopup.visible=false;
                settingsPopup.signalOkClick(appliancesComboBox.currentText);
            }
        }
        RoundButton{
            text: qsTr("Cancel");
            width: settingsColumn.width;
            font.pointSize: 15;
            icon.source: "qrc:/icons/cancel.png";
            onClicked: {
                settingsPopup.visible=false;
            }
        }
    }
}


