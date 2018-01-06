import QtQuick 2.6
import QtQuick.Window 2.2

import io.sensors.laser 1.0


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    LaserInterface
    {
        id: laser

    }

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            console.log(qsTr('Clicked on mouse area. Text: "' + textEdit.text + '"'))

            laser.transaction("/dev/tty.usbserial",1000,"*00P1\r\n")
        }

        Button
        {
            checked: false
            text: "Connect"
            anchors.top: parent

        }
    }
}
