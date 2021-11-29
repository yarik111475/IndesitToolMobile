import QtQuick 2.0

Rectangle {
    id: root;
    width: 200;
    height: 200;
    property alias running: animation.running;

    Image {
        id: img;
        fillMode: Image.PreserveAspectFit;
        anchors.fill: parent
        source: "qrc:/icons/wash.png"
        smooth: true;
        antialiasing: true;
    }

    transform: Rotation {
        id: rotation
        origin.x: img.width / 2
        origin.y: img.height / 2
        axis { x: 0; y: 1; z: 0; }
    }
    PropertyAnimation {
        id: animation;
        target: rotation
        property: "angle"
        duration: 3000
        to: 360
        from: 0;
        loops: Animation.Infinite;
        running: true;
    }

}
