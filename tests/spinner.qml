import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    width: 300
    height: 300
        BusyIndicator {
            id: busy
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            text: name
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: busy.bottom
            anchors.topMargin: 10
        }
}

