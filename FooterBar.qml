import QtQuick 2.0
import QtQuick.Controls 2.5

ToolBar{
    id: footerBar;
    width: parent.width;
    height: 50;

    signal aboutClicked();
    signal writeClicked();
    signal readClicked();
    signal scanClicked();
    signal settingsClicked();
    signal quitClicked();


    Row{
        anchors.margins: 1;
        Repeater{
            id: repeater;
            model: ["about", "write", "read", "scan","settings", "quit"];
            Rectangle{
                id: imageRectangle;
                width: footerBar.width/repeater.count;
                height: 50;
                color: "lightgray";

                Image{
                    anchors.fill: imageRectangle;
                    source: "qrc:/icons/" + modelData + ".png";
                    fillMode: Image.PreserveAspectFit;
                }
                MouseArea{
                    anchors.fill: parent;
                    onPressed: {
                        parent.color="gray";
                    }
                    onReleased: {
                        parent.color="lightgray";
                    }
                    onClicked: {
                        switch(index){
                        case 0:
                            footerBar.aboutClicked();
                            break;
                        case 1:
                            footerBar.writeClicked();
                            break;
                        case 2:
                            footerBar.readClicked();
                            break;
                        case 3:
                            footerBar.scanClicked();
                            break;
                        case 4:
                            footerBar.settingsClicked();
                            break;
                        case 5:
                            footerBar.quitClicked();
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
}
