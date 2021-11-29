import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2


ApplicationWindow {
    Connections{
        target: settingsHandler;
    }

    //Connections for eep data write/open process
    Connections{
        target: eepWriterHandler;

        //signal when open eep data file
        onSignalOpenEepDataOK: {
            clearInformationText();

            modelLabel.text=model.Model;
            codeLabel.text=model.Code;
            serialLabel.text=model.Serial;

            //check eep model checksum
            if(model.IsChecksumValid){
                checksumLabel.text=qsTr("Valid");
                checksumLabel.color="green";
            }
            else{
                checksumLabel.text=qsTr("Invalid");
                checksumLabel.color="red";
            }
        }

        //signal when exception when write/open eep data
        onSignalException: {
            messageDialog.title=qsTr("Warning");
            messageDialog.informativeText=message;
            messageDialog.visible=true;
        }
    }

    //Connections for eep data read/save process
    Connections{
        target: eepReaderHandler;

        onSignalSaveEepDataOK: {
        }

        //signal when connect to appliance OK
        onSignalReadApplianceOK: {
            applianceLabel.text=model.DeviceType;
            mcuLabel.text=model.McuType;
            firmwareLabel.text=model.FirmwareVersion;
            eepAreaSizeLabel.text=model.EepAreaSize/1024 + " Kb";
        }

        //signal when read operation starts
        onSignalReadEepStart: {
            progressIndicator.text=qsTr("Reading");
            progressIndicator.color="green";
            progressIndicator.visible=true;
        }

        //signal when read operation steps
        onSignalReadEepStep: {
        }

        //signal when read operation stops
        onSignalReadEepStop: {
            progressIndicator.value=0;
            progressIndicator.visible=false;
            swipeView.enabled=true;

            if(model.IsModelValid){
                modelLabel.text=model.Model;
                codeLabel.text=model.Code;
                serialLabel.text=model.Serial;

                //check eep model checksum
                if(model.IsChecksumValid){
                    checksumLabel.text=qsTr("Valid");
                    checksumLabel.color="green";
                }
                else{
                    checksumLabel.text=qsTr("Invalid");
                    checksumLabel.color="red";
                }


                messageDialog.title=qsTr("Information");
                messageDialog.informativeText=qsTr("Read operation completed successful");
                messageDialog.visible=true;
            }
        }

        onSignalReadLastFaultOK: {
        }

        onSignalReadFaultsBlockOK: {
        }

        //signal when exception when read/save eep data
        onSignalException: {
            messageDialog.title=qsTr("Warning");
            messageDialog.informativeText=message;
            messageDialog.visible=true;

            progressIndicator.value=0;
            progressIndicator.visible=false;
            swipeView.enabled=true;
            operationsButtonsRectangle.visible=true;
            clearInformationText();
        }
    }

    //Connections for bluetooth scanning
    Connections{
        target: bluetoothScanner;
        onSignalScanStart: {
            swipeView.enabled=false;
            settingsRectangle.visible=false;

            busyIndicator.text=qsTr("Scanning");
            busyIndicator.color="blue";
            busyIndicator.visible=true;
        }
        onSignalScanStop: {
            settingsRectangle.visible=true;
            busyIndicator.visible=false;

            swipeView.enabled=true;
            devicesComboBox.model=bluetoothScanner.getBluetoothDevicesList();
        }
        onSignalScanException: {
            busyIndicator.visible=false;
            settingsRectangle.visible=true;
            swipeView.enabled=true;

            messageDialog.title=qsTr("Warning");
            messageDialog.informativeText=message;
            messageDialog.visible=true;
        }

        onSignalConnectStart: {
            swipeView.enabled=false;
            settingsRectangle.visible=false;

            busyIndicator.text=qsTr("Connecting");
            busyIndicator.color="blue";
            busyIndicator.visible=true;
        }
        onSignalConnectStop: {
            bluetoothConnected=connected;
            settingsRectangle.visible=true;
            busyIndicator.visible=false;

            swipeView.enabled=true;
            if(bluetoothConnected){
                swipeView.currentIndex=1;
            }
            else{
                messageDialog.title=qsTr("Warning");
                messageDialog.informativeText=qsTr("Bluetooth device connection fails.Try connect to bluetooth device again");
                messageDialog.visible=true;
            }
        }
    }

    property string applianceName;
    property string bluetoothName;
    property string languageName;
    property bool bluetoothConnected;

    id: mainWindow;
    visible: true
    width: 360
    height: 600
    title: qsTr("Indesit Tool Mobile")

    header: ToolBar{
        Rectangle{
            id: headerRectangle;
            anchors.fill: parent;
            gradient: Gradient{
                GradientStop{
                    position: 0;
                    color: "black";
                }
                GradientStop{
                    position: 0.5;
                    color: "gray";
                }
                GradientStop{
                    position: 1;
                    color: "white";
                }
            }

            Label{
                id: headerLabel;
                color: "white";
                anchors.centerIn: parent;
                horizontalAlignment: Label.AlignHCenter;
                font.pointSize: 20;
                text: qsTr("Indesit Tool Mobile");
            }
        }
    }

    SwipeView {
        Component.onCompleted: { 
            bluetoothConnected=false;
        }
        id: swipeView
        anchors.fill: parent

        //settings page for select platform and blutooth device
        Page{
            id: settingsPage;
            header: ToolBar{
                width: parent.width;
                Rectangle{
                    anchors.fill: parent;
                    Text{
                        anchors.centerIn: parent;
                        horizontalAlignment: Text.AlignHCenter;
                        font.pointSize: 15;
                        text: qsTr("Settings");
                    }
                }
            }

            //settings rectangle
            Rectangle{
                id: settingsRectangle;
                width: parent.width;
                height: parent.height;

                Column{
                    anchors.fill: parent;
                    anchors.margins: 25;
                    spacing: 20;
                    Label{
                        font.pointSize: 12;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        horizontalAlignment: Label.AlignHCenter;
                        text: qsTr("Select language");
                    }
                    ComboBox{
                        id: languagesComboBox;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        model: ["ENGLISH", "RUSSIAN"];
                        onCurrentTextChanged: {
                            settingsHandler.LanguageName=currentText;
                        }
                    }

                    Label{
                        font.pointSize: 12;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        horizontalAlignment: Label.AlignHCenter;
                        text: qsTr("Select platform");
                    }
                    ComboBox{
                        id: platformsComboBox;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        model: ["ARCADIA", "EVO_2", "EVO_1"];
                        onCurrentTextChanged: {
                            settingsHandler.ApplianceName=currentText;
                        }
                    }
                    Label{
                        font.pointSize: 12;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        horizontalAlignment: Label.AlignHCenter;
                        text: qsTr("Select device");
                    }
                    ComboBox{
                        id: devicesComboBox;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        onCurrentTextChanged: {
                            settingsHandler.BluetoothName=currentText;
                        }
                    }

                    Button{
                        id: scanButton;
                        text: qsTr("Scan");
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        font.pointSize: 20;
                        onClicked: {
                            bluetoothScanner.slotStartScan();
                        }
                    }

                    Button{
                        id: connectButton;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        text: qsTr("Connect");
                        font.pointSize: 20;
                        onClicked: {
                            if(devicesComboBox.currentText != ""){
                                bluetoothName=devicesComboBox.currentText;
                                applianceName=platformsComboBox.currentText
                                bluetoothScanner.slotStartConnect(bluetoothScanner.getBluetoothAddressByName(settingsHandler.BluetoothName));
                            }
                            else{
                                messageDialog.title=qsTr("Warning");
                                messageDialog.informativeText=qsTr("Select device and platform for connection");
                                messageDialog.visible=true;
                            }
                        }
                    }

                    Button{
                        id: exitButton;
                        anchors.horizontalCenter: parent.horizontalCenter;
                        width: parent.width/1.5;
                        text: qsTr("Exit");
                        font.pointSize: 20;
                        onClicked: {
                            Qt.quit();
                        }
                    }
                }
            }
        }

        //operations page for read/write operations
        Page{
            id: operationsPage;
            header: ToolBar{
                width: parent.width;
                Rectangle{
                    anchors.fill: parent;
                    Text{
                        anchors.centerIn: parent;
                        horizontalAlignment: Text.AlignHCenter;
                        font.pointSize: 15;
                        text: qsTr("Operations");
                    }
                }
            }
            //column to display operations buttons and full information of appliance
            Column{
                spacing: 20;
                anchors.centerIn: parent;

                //read-write buttons row
                Row{
                    spacing: 30;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    Button{
                        id: readButton;
                        font.pointSize: 20;
                        text:qsTr("Read");
                        onClicked: {
                            if(!bluetoothConnected){
                                messageDialog.title=qsTr("Waring");
                                messageDialog.informativeText=qsTr("First you must set connection to bluetooth device");
                                messageDialog.visible=true;
                            }
                            else{
                                eepReaderHandler.init(bluetoothScanner.getBluetoothAddressByName(settingsHandler.BluetoothName),
                                                      settingsHandler.ApplianceName);
                                eepReaderHandler.slotReadEep();
                            }
                        }
                    }

                    Button{
                        id: writeButton;
                        text: "Write";
                        font.pointSize: 20;
                        onClicked: {
                            if(!bluetoothConnected){
                                messageDialog.title=qsTr("Waring");
                                messageDialog.informativeText=qsTr("First you must set connection to bluetooth device");
                                messageDialog.visible=true;
                            }
                            else{
                                openFileDialog.visible=true;
                            }
                        }
                    }
                }

                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Appliance:");
                    }
                    Label{
                        id: applianceLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Mcu:");
                    }
                    Label{
                        id: mcuLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Firmware:");
                    }
                    Label{
                        id: firmwareLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Eep size:");
                    }
                    Label{
                        id: eepAreaSizeLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Model:");
                    }
                    Label{
                        id: modelLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Code:");
                    }
                    Label{
                        id: codeLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Serial:");
                    }
                    Label{
                        id: serialLabel;
                        font.pointSize: 15;
                    }
                }
                Row{
                    spacing: 20;
                    Label{
                        font.pointSize: 15;
                        text: qsTr("Checksum:");
                    }
                    Label{
                        id: checksumLabel;
                        font.pointSize: 15;
                    }
                }
            }
        }

        //statistics/failures page
        Page{
            id: statisticsPage;
            header: ToolBar{
                width: parent.width;
                Rectangle{
                    anchors.fill: parent;
                    Text{
                        anchors.centerIn: parent;
                        horizontalAlignment: Text.AlignHCenter;
                        font.pointSize: 15;
                        text: qsTr("Statistics");
                    }
                }
            }
        }
    }

    MessageDialog{
        id: messageDialog;
        visible: false;
        modality: Qt.WindowModal;
    }

    FileDialog{
        id: openFileDialog;
        title: qsTr("Open file");
        modality: Qt.WindowModal;
        selectExisting: true;
        selectMultiple: false;
        nameFilters: ["eep files (*.eep)", "bin files (*.bin)"];
        visible: false;
        onAccepted: {

        }
    }

    FileDialog{
        id: saveFileDialog;
        title: qsTr("Save file");
        modality: Qt.WindowModal;
        selectExisting: false;
        nameFilters: ["eep files (*.eep)", "bin files (*.bin)"];
        visible: false;
    }

    Rectangle{
        id: busyIndicator;
        property alias text: busyLabel.text;
        property alias color: busyLabel.color;
        anchors.fill: parent;
        visible: false;
        Column{
            anchors.centerIn: parent;
            anchors.margins: 20;
            spacing: 10;

            BusyIndicator{
                width: mainWindow.width/3;
                height: mainWindow.height/3;
                anchors.horizontalCenter: parent.horizontalCenter;
            }
            Label{
                id: busyLabel;
                font.pointSize: 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                horizontalAlignment: Label.AlignHCenter;
            }
        }
    }

    Rectangle{
        id: progressIndicator;
        anchors.fill: parent;
        visible: false;

        property alias text: progressLabel.text;
        property alias from: progressBar.from;
        property alias to: progressBar.to;
        property alias value: progressBar.value;
        property alias color: progressLabel.color;

        Column{
            anchors.centerIn: parent;
            anchors.margins: 10;
            spacing: 10;

            ProgressBar{
                id: progressBar;
                indeterminate: true;
                anchors.horizontalCenter: parent.horizontalCenter;
            }
            Label{
                id: progressLabel;
                font.pointSize: 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                horizontalAlignment: Label.AlignHCenter;
            }
        }

    }

    PageIndicator{
        id: pageIndicator;
        count: swipeView.count;
        currentIndex: swipeView.currentIndex;
        anchors.bottom: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;
    }

    //function to clear all information labels
    function clearInformationText(){
        applianceLabel.text="";
        mcuLabel.text=""
        firmwareLabel.text=""
        eepAreaSizeLabel.text=""

        modelLabel.text="";
        codeLabel.text="";
        serialLabel.text="";
        checksumLabel.text="";
    }
}
