import QtQuick 2.0

ListModel {
    ListElement{
        type: qsTr("Current fault");
        code: "01";
        subcode: "--";
        occurence: "--";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Faults");
        code: "01";
        subcode: "04";
        occurence: "09";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Faults");
        code: "00";
        subcode: "00";
        occurence: "00";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Faults");
        code: "00";
        subcode: "00";
        occurence: "00";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Prefaults");
        code: "00";
        subcode: "00";
        occurence: "00";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Prefaults");
        code: "00";
        subcode: "00";
        occurence: "00";
        description: "No description presents";
    }
    ListElement{
        type: qsTr("Prefaults");
        subcode: "00";
        occurence: "00";
        description: "No description presents";
    }

}
