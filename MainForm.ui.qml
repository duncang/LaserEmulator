import QtQuick 2.6

Rectangle {
    property alias mouseArea: mouseArea
    property alias textEdit: textEdit

    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        hoverEnabled: false
        anchors.rightMargin: 21
        anchors.bottomMargin: 222
        anchors.leftMargin: 143
        anchors.topMargin: 76
        anchors.fill: parent

        Text {
            id: text1
            x: 20
            y: 14
            width: 146
            height: 30
            text: qsTr("SEND")
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            style: Text.Normal
            font.pixelSize: 8
            font.family: "Tahoma"
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        anchors.horizontalCenterOffset: -13
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 190
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            anchors.rightMargin: -119
            anchors.bottomMargin: -123
            anchors.leftMargin: -89
            anchors.topMargin: -17
            border.width: 1
        }
    }
}
