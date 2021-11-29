import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    id: root;
    modal: true;
    topInset: -5;
    bottomInset: -5;
    leftInset: -5;
    rightInset: -5;
    property int itemWidth: 100;
    property int itemHeight: 70;
    closePolicy: Popup.CloseOnPressOutside;

    onAboutToShow: {
        animation.running=true;
    }
    onAboutToHide: {
        animation.running=false;
    }

    PropertyAnimation{
        id: animation;
        properties: "opacity";
        target: root;
        from: 0;
        to: 1;
        duration: 300;
        running: false;
    }


//    signal signalScan();
//    signal signalRead();
//    signal signalWrite();
//    signal signalOpen();
//    signal signalSave();
//    signal signalSettings();
//    signal signalAbout();
//    signal signalQuit();

    background: Rectangle{
        color: "transparent";
    }

    ListModel{
        id: dataModel;
        ListElement{
            name: "save";
            value: qsTr("Save");
        }
        ListElement{
            name: "settings";
            value: qsTr("Settings");
        }
        ListElement{
            name: "about";
            value: qsTr("About");
        }
        ListElement{
            name: "quit";
            value: qsTr("Quit");
        }
        ListElement{
            name: "scan";
            value: qsTr("Scan");
        }
        ListElement{
            name: "read";
            value: qsTr("Read");
        }
        ListElement{
            name: "write";
            value: qsTr("Write");
        }
        ListElement{
            name: "open";
            value: qsTr("Open");
        }
    }

    contentItem: PathView{
        id: view;
        clip: true;
        snapMode: PathView.SnapOneItem;
        anchors.fill: parent;

        model: dataModel;

        path: Path{
            startX: view.width/2;
            startY: view.height-itemHeight;

            PathAttribute{name: "enabled"; value: 0;}
            PathAttribute{name: "fontSize"; value: 10;}
            PathAttribute{name: "imageSize"; value: 25;}
            PathAttribute{name: "width"; value: itemWidth;}
            PathAttribute{name: "height"; value: itemHeight;}
            PathAttribute{ name: "opacity"; value: 1.0;}
            PathAttribute{ name: "z"; value: 0;}

            PathArc{
                radiusX: root.width/6;
                radiusY: root.height/6;
                x:root.width/2;
                y: itemHeight+10;
            }

            PathPercent{ value: 0.49;}

            PathAttribute{name: "enabled"; value: 1;}
            PathAttribute{name: "fontSize"; value: 10;}
            PathAttribute{name: "imageSize"; value: 25;}
            PathAttribute{name: "width"; value: itemWidth;}
            PathAttribute{name: "height"; value: itemHeight;}
            PathAttribute{ name: "opacity"; value: 1.0;}
            PathAttribute{ name: "z"; value: 1;}

            PathLine{
                x: root.width/2
                y: itemHeight+10;
            }

            PathAttribute{name: "enabled"; value: 1;}
            PathAttribute{name: "fontSize"; value: 10;}
            PathAttribute{name: "imageSize"; value: 25;}
            PathAttribute{name: "width"; value: itemWidth;}
            PathAttribute{name: "height"; value: itemHeight;}
            PathAttribute{ name: "opacity"; value: 1.0;}
            PathAttribute{ name: "z"; value: 1;}
            PathPercent{ value: 0.51; }

            PathArc{
                radiusX: root.width/6;
                radiusY: root.height/6;
                x:root.width/2;
                y: root.height-itemHeight-10;
            }

            PathAttribute{name: "enabled"; value: 0;}
            PathAttribute{name: "fontSize"; value: 10;}
            PathAttribute{name: "imageSize"; value: 25;}
            PathAttribute{name: "state"; value: 0;}
            PathAttribute{name: "width"; value: itemWidth;}
            PathAttribute{name: "height"; value: itemHeight;}
            PathAttribute{ name: "opacity"; value: 1.0;}
            PathAttribute{ name: "z"; value: 0;}
            PathPercent{ value: 1;}
        }

        delegate: Rectangle{
            id: itemDelegate;
            property var view: PathView.view;
            property var enabled: PathView.enabled;
            property var fontSize: PathView.fontSize;
            property var imageSize: PathView.imageSize;
            width: PathView.width;
            height: PathView.height;
            opacity: PathView.opacity;
            color: "lightsteelblue";
            z: PathView.z;
            anchors.margins: 10;


            border{
                color: "black";
                width: 1;
            }
            radius: 5;

            Column{
                anchors.centerIn: parent;
                anchors.margins: 5;
                spacing: 5;
                Image{
                    id: img;
                    width: imageSize;
                    height: imageSize;
                    fillMode: Image.PreserveAspectFit;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    source: "qrc:/icons/" + model.name + ".png";

                    transform: Rotation{
                        id: rotation
                        origin.x: img.width / 2
                        origin.y: img.height / 2
                        axis { x: 0; y: 1; z: 0; }
                    }
                    PropertyAnimation {
                        id: imgAnimation;
                        target: rotation
                        property: "angle"
                        duration: 3000
                        to: 360
                        from: 0;
                        loops: Animation.Infinite;
                        running: false;
                    }
                }
                Text{
                    horizontalAlignment: Text.AlignHCenter;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    text: model.value;
                    renderType: Text.NativeRendering;
                    font.pointSize: fontSize;
                }
            }

            MouseArea{
                anchors.fill: parent;
                onClicked: {
                    root.visible=false;
                    menuTriggered(model.index);
//                    if(itemDelegate.enabled===1){
//                        root.visible=false;
//                        menuTriggered(model.index);
//                    }
                }
            }
        }
    }

    function menuTriggered(index){
        switch(index){
        case 0:
            root.signalSave();
            break;
        case 1:
            root.signalSettings();
            break;
        case 2:
            root.signalAbout();
            break;
        case 3:
            root.signalQuit();
            break;
        case 4:
            root.signalScan();
            break;
        case 5:
            root.signalRead();
            break;
        case 6:
            root.signalWrite();
            break;
        case 7:
            root.signalOpen();
            break;
        }
    }
}
