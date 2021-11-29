import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Extras 1.4
import com.ServiceToolMobile 1.0


//Main application window
ApplicationWindow {
    id: mainWindow;
    visible: true
    width: 400
    height: 640
    title: qsTr("Service Tool Mobile")
    property bool writeCheck: false;

    SQLiteManager{
        id: sqliteManager;
    }

    SettingsHandler{
        id:settingsHandler;

        onApplianceNameChanged: {
            if(settingsHandler.isActivated){
                updateDatabaseView();
            }
        }

        onIsActivatedChanged: {
            if(activated){
                updateDatabaseView();
            }
        }
    }

    BluetoothScanner{
        id: bluetoothScanner;
        onSignalScanStart: {
            busyDialog.text=qsTr("Scan");
            busyDialog.color="mediumblue";
            busyDialog.visible=true;
        }

        onSignalScanStop: {
            busyDialog.visible=false;

            settingsDialog.bluetoothModel=getBluetoothDevicesList();
            settingsDialog.visible=true;
        }

        onSignalScanException: {
            busyDialog.visible=false;

            messageDialog.source="qrc:/icons/warning.png";
            messageDialog.text=message;
            messageDialog.visible=true;
        }

        onSignalConnectStart: {
            busyDialog.text=qsTr("Connect");
            busyDialog.color="mediumblue";
            busyDialog.visible=true;
        }

        onSignalConnectStop: {
            settingsHandler.bluetoothConnected=connected;
            if(connected){
                settingsHandler.requestActivation(bluetoothAddress);
            }

            headerBar.status=connected;
            busyDialog.visible=false;
            if(settingsHandler.bluetoothConnected){
                messageDialog.source="qrc:/icons/information.png";
                messageDialog.text=qsTr("Bluetooth connection OK\n Use Read/Write operations");
                messageDialog.visible=true;
            }
            else{
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.text=qsTr("Bluetooth device connection fails\nTry connect to bluetooth device again");
                messageDialog.visible=true;
            }
        }
    }

    //Header with menu
    header: HeaderBar{
        id: headerBar;

        onSignalScan: {
            bluetoothScanner.slotStartScan();
        }

        onSignalRead: {
            if(settingsHandler.bluetoothConnected){
                readEepData();
            }
            else{
                messageDialog.text=qsTr("First you must set\n connection to bluetooth device");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true;
            }
        }

        onSignalWrite: {
            if(settingsHandler.bluetoothConnected){
                if(settingsHandler.isActivated){
                    writeEepData();
                }
                else{
                    messageDialog.text=qsTr("That function is not available\nin Demo version");
                    messageDialog.source="qrc:/icons/warning.png";
                    messageDialog.visible=true;
                }
            }
            else{
                messageDialog.text=qsTr("First you must set\n connection to bluetooth device");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true;
            }
        }

        onSignalOpen: {
            openFileDialog.applianceName = settingsHandler.applianceName;
            openFileDialog.visible=true;
        }

        onSignalSave: {
            if(fileHandler.isEepModelValid()){
                var eepModelName=fileHandler.getEepModelName();
                fileHandler.saveEepModelData(settingsHandler.applianceName,
                                             eepModelName, "*.eep");
            }
            else{
                messageDialog.text=qsTr("No eep data exists to save\nFirst you need read or open file");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true
            }
        }

        onSignalSaveToDb: {
            if(fileHandler.isEepModelValid()){
                if(settingsHandler.isActivated){
                    fileHandler.addEepModel(settingsHandler.applianceName);
                }
                else{
                    messageDialog.text=qsTr("That function is not available\nin Demo version");
                    messageDialog.source="qrc:/icons/warning.png";
                    messageDialog.visible=true;
                }
            }
            else{
                messageDialog.text=qsTr("No eep data exists to save\nFirst you need read or open file");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true
            }
        }

        onSignalSettings: {
            settingsDialog.visible=true;
        }

        onSignalAbout: {
            aboutDialog.visible=true;
        }

        onSignalQuit: {
            Qt.quit();
        }
    }

    //Menu dialog to show main application menu
    /* MenuDialog{
        id: menuDialog;
        anchors.centerIn: parent;
        width: parent.width-20;
        height: parent.height-80;
        visible: false;

        onSignalScan: {
            bluetoothScanner.slotStartScan();
        }

        onSignalRead: {
            if(settingsHandler.BluetoothConnected){
                readEepData();
            }
            else{
                messageDialog.text=qsTr("First you must set\n connection to bluetooth device");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true;
            }
        }

        onSignalWrite: {
            if(settingsHandler.BluetoothConnected){
                if(settingsHandler.IsActivated){
                    writeEepData();
                }
                else{
                    messageDialog.text=qsTr("That function is not available\nin Demo version");
                    messageDialog.source="qrc:/icons/warning.png";
                    messageDialog.visible=true;
                }
            }
            else{
                messageDialog.text=qsTr("First you must set\n connection to bluetooth device");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true;
            }
        }

        onSignalOpen: {
            openFileDialog.applianceName = settingsHandler.ApplianceName;
            openFileDialog.visible=true;
        }

        onSignalSave: {
            if(fileHandler.isEepModelValid()){
                var eepModelName=fileHandler.getEepModelName();
                fileHandler.saveEepModelData(settingsHandler.ApplianceName,
                                             eepModelName, "*.eep");
            }
            else{
                messageDialog.text=qsTr("No eep data exists to save\nFirst you need read or open file");
                messageDialog.source="qrc:/icons/warning.png";
                messageDialog.visible=true
            }
        }

        onSignalSettings: {
            settingsDialog.visible=true;
        }

        onSignalAbout: {
            aboutDialog.visible=true;
        }

        onSignalQuit: {
            Qt.quit();
        }
    }
    */

     //Swipe view to show information about appliance and appliance's faulures
     SwipeView{
         id: swipeView;
         currentIndex: 0;
         anchors{
             top: parent.top;
             left: parent.left;
             right: parent.right;
             bottom: pageIndicator.top;
         }

         //Appliance page
         Page{
             id: appliancePage;
             header: Item{
                 width: swipeView.width;
                 height: 50;
                 Rectangle{
                     anchors.fill: parent;
                     anchors.margins: 5;
                     radius: 5;
                     color: "steelblue";
                     Text{
                         font.pointSize: 20;
                         color: "white";
                         renderType: Text.NativeRendering;
                         anchors.centerIn: parent;
                         horizontalAlignment: Text.AlignHCenter;
                         text: qsTr("Information");
                     }
                 }
             }

             ListView{
                 id: applianceListView;
                 anchors.margins: 5;
                 anchors.fill: parent;
                 spacing: 5;
                 clip: true;
                 z: 1;

                 section.property: "type";
                 section.delegate: Rectangle{
                     width: applianceListView.width;
                     height: 35;
                     Text{
                         anchors.centerIn: parent;
                         text: section;
                         font.pointSize: 20;
                         renderType: Text.NativeRendering;
                     }
                 }

                 model: applianceModel;
                 delegate: ApplianceDelegate{}
             }

             LogoDialog{
                 z: 0;
                 anchors.centerIn: parent;
             }
         }

         //Statistics page
         Page{
             id: statisticsPage;
             header: Item{
                 width: swipeView.width;
                 height: 50;
                 Rectangle{
                     id: headerRectangle;
                     anchors.fill: parent;
                     anchors.margins: 5;
                     color: "steelblue";
                     radius: 5;
                     Text{
                         font.pointSize: 20;
                         color: "white";
                         renderType: Text.NativeRendering;
                         anchors.centerIn: parent;
                         horizontalAlignment: Text.AlignHCenter;
                         text: qsTr("Statistics");
                     }
                 }
             }

             Text{
                 id: statisticsDemoText;
                 visible: !settingsHandler.isActivated;
                 anchors.top: parent.top;
                 anchors.left: parent.left;
                 anchors.right: parent.right;
                 z: 1;

                 font.pointSize: 20;
                 renderType: Text.NativeRendering;
                 horizontalAlignment: Text.AlignHCenter;
                 text: qsTr("That function not available\nin Demo version");
             }

             ListView{
                 id: statisticsListView;
                 anchors.margins: 5;
                 anchors.fill: parent;
                 spacing: 5;
                 clip: true;
                 visible: settingsHandler.isActivated;
                 z: 1;

                 section.property: "type";
                 section.delegate: Rectangle{
                     width: statisticsListView.width;
                     height: 35;
                     Text{
                         anchors.centerIn: parent;
                         text: section;
                         font.pointSize: 20;
                         renderType: Text.NativeRendering;
                     }
                 }

                 model: faultsModel;
                 delegate: FaultDelegate{
                     onSignalViewDescription: {
                         faultDialog.text = description;
                         faultDialog.visible=true;
                     }
                 }
             }

             LogoDialog{
                 z: 0;
                 anchors.centerIn: parent;
             }
         }

         //Database page
         Page{
             header: Item{
                 width: swipeView.width;
                 height: 50;
                 Rectangle{
                     anchors.fill: parent;
                     anchors.margins: 5;
                     color: "steelblue";
                     radius: 5;
                     Text{
                         font.pointSize: 20;
                         color: "white";
                         renderType: Text.NativeRendering;
                         anchors.centerIn: parent;
                         horizontalAlignment: Text.AlignHCenter;
                         text: qsTr("Database");
                     }
                 }
             }

             Text{
                 visible: !settingsHandler.isActivated;
                 anchors.top: parent.top;
                 anchors.left: parent.left;
                 anchors.right: parent.right;

                 font.pointSize: 20;
                 renderType: Text.NativeRendering;
                 horizontalAlignment: Text.AlignHCenter;
                 text: qsTr("That function not available\nin Demo version");
             }

             ListView{
                 id: databaseView;
                 anchors.fill: parent;
                 anchors.margins: 5;
                 spacing: 5;
                 clip: true;
                 visible: settingsHandler.isActivated;
                 z: 1;

                 model: eepModel;

                 delegate: Rectangle{
                     width: databaseView.width;
                     height: 120;
                     color: "lightsteelblue";
                     radius: 5;


                     Column{
                         anchors.fill: parent;
                         anchors.margins: 10;
                         spacing: 5;

                         Text{
                             text: qsTr("Model: ") + model.name;
                             font.pointSize: 15;
                             renderType: Text.NativeRendering;
                         }
                         Text{
                             text: qsTr("Code: ") + model.code;
                             font.pointSize: 15;
                             renderType: Text.NativeRendering;
                         }
                         Text{
                             text: qsTr("Serial: ") + model.serial;
                             font.pointSize: 15;
                             renderType: Text.NativeRendering;
                         }
                     }

                     MouseArea{
                         anchors.fill: parent;
                         onClicked: {
                             fileHandler.openEepModelDataFromDatabase(settingsHandler.applianceName, model.name, model.code, model.serial);
                             swipeView.setCurrentIndex(0);
                         }
                     }
                 }
             }

             LogoDialog{
                 z: 0;
                 anchors.centerIn: parent;
             }
         }
     }

     //Connections for  save/open eep data
     Connections{
         target: fileHandler;

         onSignalSaveEepDataOK: {
             messageDialog.text=qsTr("Eep data saved success");
             messageDialog.source="qrc:/icons/information.png";
             messageDialog.visible=true
         }

         onSignalOpenEepDataOK: {
             //Clear appliance model
             if(applianceModel.rowCount()!==0){
                 applianceModel.remove(0, applianceModel.rowCount());
             }
             //Fill data for appliance
             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Device");
             applianceElement.value="";
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Mcu");
             applianceElement.value="";
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Firmware");
             applianceElement.value="";
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Eep size");
             applianceElement.value="";
             applianceModel.append(applianceElement);

             //Fill data for eep
             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Model");
             applianceElement.value=model.Model;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Code");
             applianceElement.value=model.Code;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Serial");
             applianceElement.value=model.Serial;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Checksum");
             applianceElement.value=model.IsChecksumValid ? "Valid" : "Invalid";
             applianceModel.append(applianceElement);

             //Set current index swipeView to 0
             swipeView.currentIndex=0;

             messageDialog.text=qsTr("Eep data opened success");
             messageDialog.source="qrc:/icons/information.png";
             messageDialog.visible=true
         }

         onSignalAddEepDataOK: {
             updateDatabaseView();
             messageDialog.text=qsTr("Eep data added \ninto database success");
             messageDialog.source="qrc:/icons/information.png";
             messageDialog.visible=true
         }

         onSignalException: {
             messageDialog.source="qrc:/icons/warning.png";
             messageDialog.text=message;
             messageDialog.visible=true
         }
     }

     //Connections for eep data write process
     Connections{
         target: eepWriterHandler;

         onSignalEraseEep: {
             progressDialog.visible=true;
             progressDialog.text=qsTr("Erase");
             progressDialog.color="mediumblue";
             progressDialog.indeterminate=true;
         }

         onSignalWriteEepStart: {
             writeCheck=true;
             progressDialog.visible=true;
             progressDialog.text=qsTr("Write");
             progressDialog.color="red";
             progressDialog.from=0;
             progressDialog.to=size;
             progressDialog.value=0;
             progressDialog.indeterminate=false;
         }

         onSignalWriteEepStep: {
             progressDialog.value+=step;
         }

         onSignalWriteEepStop: {
         }

         onSignalIODeviceDisconnected: {
             if(writeCheck){
                 readEepData(writeCheck);
             }
         }

         //signal when exception when write/open eep data
         onSignalException: {
             progressDialog.visible=false;
             writeCheck=false;

             messageDialog.source="qrc:/icons/warning.png";
             messageDialog.text=message;
             messageDialog.visible=true;
         }
     }

     //Connections for eep data read process
     Connections{
         target: eepReaderHandler;

         //signal when read operation starts
         onSignalReadEepStart: {
             progressDialog.visible=true;
             progressDialog.text= writeCheck ? qsTr("Check") : qsTr("Read");
             progressDialog.color="green";
             progressDialog.from=0;
             progressDialog.to=size;
             progressDialog.value=0;
         }

         //signal when read operation steps
         onSignalReadEepStep: {
             progressDialog.value+=step/2;
         }

         //signal when read operation stops
         onSignalReadEepStop: {
             progressDialog.visible=false;

             if(writeCheck){
                 messageDialog.text=qsTr("Write operation\n completed success");
             }
             else{
                 messageDialog.text=qsTr("Read operation\n completed success");
             }

             messageDialog.source="qrc:/icons/information.png";
             messageDialog.visible=true;

             //Clear appliance model
             if(applianceModel.rowCount()!==0){
                 applianceModel.remove(0, applianceModel.rowCount());
             }

             //Fill data for appliance
             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Device");
             applianceElement.value=model.DeviceType;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Mcu");
             applianceElement.value=model.McuType;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Firmware");
             applianceElement.value=model.FirmwareVersion;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Appliance");
             applianceElement.name=qsTr("Eep size");
             applianceElement.value=model.EepAreaSize;
             applianceModel.append(applianceElement);

             //Fill data for eep
             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Model");
             applianceElement.value=model.Model;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Code");
             applianceElement.value=model.Code;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Serial");
             applianceElement.value=model.Serial;
             applianceModel.append(applianceElement);

             applianceElement.type=qsTr("Configuration");
             applianceElement.name=qsTr("Checksum");
             applianceElement.value=model.IsChecksumValid ? "Valid" : "Invalid";
             applianceModel.append(applianceElement);

             writeCheck=false;

             //Set current index swipeView to 0
             swipeView.currentIndex=0;
         }

         onSignalIODeviceDisconnected: {
         }

         onSignalReadLastFaultOK: {
             if(settingsHandler.isActivated){
                 //Clear faults model
                 if(faultsModel.rowCount()!==0){
                     faultsModel.remove(0, faultsModel.rowCount());
                 }

                 faultElement.clear=faultModel.IsFaultClear;
                 faultElement.type=qsTr("Last fault");
                 faultElement.code=faultModel.FaultCode;
                 faultElement.subcode=faultModel.SubCode;
                 faultElement.occurence=faultModel.Occurence;
                 faultElement.description=faultModel.Description;

                 faultsModel.append(faultElement);
             }
         }

         onSignalReadFaultsBlockOK: {
             if(settingsHandler.isActivated){
                 if(faultsList.length===5){
                     fillShortFaults(faultsList);
                 }
                 else if(faultsList.length===10){
                     fillFullFaults(faultsList);
                 }
                 else if(faultsList.length===16){
                     fillShortFaults(faultsList);
                 }
             }
             else{
                 statisticsDemoText.visible=true;
             }
         }

         //signal when exception when read/save eep data
         onSignalException: {
             progressDialog.visible=false;

             messageDialog.source="qrc:/icons/warning.png";
             messageDialog.text=message;
             messageDialog.visible=true;
             writeCheck=false;
         }
     }

     //Application about dialog
     AboutDialog{
         id: aboutDialog
         anchors.centerIn: parent;
         applicationName: settingsHandler.getApplicationName();
         applicationVersion: qsTr("Version: ") + settingsHandler.getApplicationVersion();
         visible: false;
     }

     //Application settings dialog
     SettingsDialog{
         id: settingsDialog;
         anchors.centerIn: parent;
         width: mainWindow.width/1.5;

         onSignalRescanClick: {
             bluetoothScanner.slotStartScan();
         }

         onSignalConnectClick: {
             settingsHandler.applianceName=applianceName;

             if(bluetoothName !== ""){
                 bluetoothScanner.slotStartConnect(bluetoothName);
             }
             else{
                 messageDialog.source="qrc:/icons/warning.png";
                 messageDialog.text=qsTr("First you must select device\nand platform for connection");
                 messageDialog.visible=true;
             }
         }

         onSignalOkClick: {
             settingsHandler.applianceName=applianceName;
         }

         onApplianceNameChanged: {
             settingsHandler.applianceName=applianceName;
         }
     }

     //Open file dialog
     OpenFileDialog{
         id: openFileDialog;
         width: parent.width-100;
         height: parent.height-100;
         anchors.centerIn: parent;

         onSignalSelect: {
             fileHandler.openEepModelData(settingsHandler.applianceName, selectedFileName);
         }
     }

     //Progress dialog for read/write operations
     ProgressDialog{
         id: progressDialog;
         visible: false;
         width: parent.width-20;
         anchors.centerIn: parent;

     }

     //Busy dialog
     BusyDialog{
         id: busyDialog;
         visible: false;
         width: parent.width-20;
         height: 130
         anchors.centerIn: parent;
     }

     //Information dialog
     InformationDialog{
         id: messageDialog;
         visible: false;
         anchors.centerIn: parent;
         width: parent.width-20;
     }

     //Fault dialog to show fault description
     FaultDialog{
         id: faultDialog;
         width: parent.width/1.5;
         anchors.centerIn: parent;
         visible: false;

         onVisibleChanged: {
             animation.running=visible;
         }

         PropertyAnimation{
             id: animation;
             target: faultDialog;
             properties: "opacity";
             duration: 300;
             from: 0;
             to: 1;
             running: false;
         }

     }

     //Page indicator for swipeView
     PageIndicator{
         id: pageIndicator;
         anchors.bottom: parent.bottom;
         anchors.horizontalCenter: parent.horizontalCenter;
         count: swipeView.count;
         currentIndex: swipeView.currentIndex;
     }

     //Eep proxy model to display full data (appliance model data and eep model data)
     ListModel{
         id: eepProxyModel;
     }

     //Faults model
     ListModel{
         id: faultsModel;
     }

     ListModel{
         id: eepModel;
     }
     ListElement{
         id: eepElement;
         property string name;
         property string code;
         property string serial;

     }

     //ListElement for faultsModel
     ListElement{
         id: faultElement;
         property bool clear;
         property string type;
         property string code;
         property string subcode;
         property string occurence;
         property string description;
     }

     //Appliance and configuration model
     ListModel{
         id: applianceModel;
     }

     //ListElement for appliance and configuration model
     ListElement{
         id: applianceElement;
         property string type;
         property string name;
         property string value;
     }

     //Function to read eep data
     function readEepData(writeCheck){
         eepReaderHandler.init(settingsHandler.applianceName, bluetoothScanner.bluetoothAddress, writeCheck);
         eepReaderHandler.slotReadEep(writeCheck);
     }

     //Function to write eep data
     function writeEepData(){
         eepWriterHandler.init(settingsHandler.applianceName, bluetoothScanner.bluetoothAddress);
         eepWriterHandler.slotWriteEep();
     }

     //Function for fill short statistics list(5 faults)
     function fillShortFaults(faultsList){
         for(var i=0;i<faultsList.length;i++){
             faultElement.clear=faultsList[i].IsFaultClear;
             faultElement.type=qsTr("Faults");
             faultElement.code=faultsList[i].FaultCode;
             faultElement.subcode=faultsList[i].SubCode;
             faultElement.occurence=faultsList[i].Occurence;
             faultElement.description=faultsList[i].Description;

             faultsModel.append(faultElement);
         }
     }

     //Function for fill full statistics list(all faults)
     function fillFullFaults(faultsList){
         for(var i=0;i<faultsList.length/2;i++){
             faultElement.clear=faultsList[i].IsFaultClear;
             faultElement.type=qsTr("Faults");
             faultElement.code=faultsList[i].FaultCode;
             faultElement.subcode=faultsList[i].SubCode;
             faultElement.occurence=faultsList[i].Occurence;
             faultElement.description=faultsList[i].Description;

             faultsModel.append(faultElement);
         }
         for(var j=5;j<faultsList.length;j++){
             faultElement.clear=faultsList[j].IsFaultClear;
             faultElement.type=qsTr("Prefaults");
             faultElement.code=faultsList[j].FaultCode;
             faultElement.subcode=faultsList[j].SubCode;
             faultElement.occurence=faultsList[j].Occurence;
             faultElement.description=faultsList[j].Description;

             faultsModel.append(faultElement);
         }
     }

     function updateDatabaseView(){
         if(fileHandler.isDatabaseCorrect){
             if(eepModel.rowCount() > 0){
                 eepModel.remove(0, eepModel.rowCount());
             }

             var list=fileHandler.getEepModelsList(settingsHandler.applianceName);
             for(var i=0; i<list.length; i++){
                 eepElement.name=list[i].Model;
                 eepElement.code=list[i].Code;
                 eepElement.serial=list[i].Serial;

                 eepModel.append(eepElement);
             }
         }
     }
}
