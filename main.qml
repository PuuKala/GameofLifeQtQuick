import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 6.2

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Button {
        id: startButton
        x: 5
        y: 445
        width: 75
        height: 30
        text: qsTr("Start")
        onClicked: {
            gameTimer.running = !gameTimer.running;
            if(gameTimer.running)
            {
                this.text = "Stop";
            }
            else
            {
                this.text = "Start";
            }
        }
    }

    Button {
        id: nextButton
        x: 90
        y: 445
        width: 75
        height: 30
        text: qsTr("Next step")
        onClicked:
        {
            gameOfLifeImage.source = "";
            gameOfLifeImage.source = "image://gameoflife/r";
        }
    }

    Image {
        id: gameOfLifeImage
        x: 5
        y: 5
        width: 630
        height: 435
        source: "image://gameoflife/"
        smooth: false
        cache: false
        fillMode: Image.PreserveAspectFit

        MouseArea {
            id: gameOfLifeMouseArea
            anchors.fill: parent
            onClicked: (mouse)=> {
                           gameOfLifeImage.source = "image://gameoflife/p" + mouse.x + "x" + mouse.y;
                       }
        }
    }

    Timer {
        id: gameTimer
        running: false
        repeat: true
        interval: 200
        onTriggered:
        {
            gameOfLifeImage.source = "";
            gameOfLifeImage.source = "image://gameoflife/r";
        }
    }

    TextInput {
        id: heightInput
        x: 360
        y: 450
        width: 80
        height: 20
        text: qsTr("Height")
        font.pixelSize: 12
    }

    TextInput {
        id: widthInput
        x: 479
        y: 450
        width: 80
        height: 20
        text: qsTr("Width")
        font.pixelSize: 12
    }

    Text {
        id: xText
        x: 456
        y: 452
        width: 10
        height: 16
        text: qsTr("x")
        font.pixelSize: 12
    }

    Button {
        id: applySizeButton
        x: 560
        y: 445
        width: 75
        height: 30
        text: qsTr("Apply size")
        onClicked: gameOfLifeImage.source = "image://gameoflife/s" + widthInput.text + "x" + heightInput.text
    }

    TextInput {
        id: pointInput
        x: 171
        y: 452
        width: 100
        height: 20
        text: qsTr("XCoordxYCoord")
        font.pixelSize: 12
    }

    Button {
        id: setPointButton
        x: 275
        y: 445
        width: 75
        height: 30
        text: qsTr("Set point")
        onClicked:
        {
            gameOfLifeImage.source = "";
            gameOfLifeImage.source = "image://gameoflife/p" + pointInput.text;
        }
    }
}
