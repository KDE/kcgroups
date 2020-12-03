// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

import QtQuick 2.0
import QtQuick.Controls 2.0

ListView {
    width: parent && parent.width > 1000 ? parent.width : 1000
    height: parent && parent.height > 600 ? parent.height : 600
    model: appList
    delegate: Rectangle {
        id: rect
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        color: index % 2 ? "lightblue": "lightgreen"

        Text {
            id: indexText
            anchors.left: rect.left
            anchors.verticalCenter: rect.verticalCenter
            anchors.leftMargin: 10
            text: index
            width: 50
        }

        CheckBox {
            id: quotaCheckBox
            anchors.left: indexText.right
            anchors.verticalCenter: rect.verticalCenter
            checked: object.cpuQuota.hasValue
            onToggled: object.cpuQuota.value = checked ? quotaSlider.value : undefined
        }

        Slider {
            id: quotaSlider
            anchors.left: quotaCheckBox.right
            anchors.verticalCenter: rect.verticalCenter
            width: 200
            anchors.leftMargin: 10
            from: 0
            to: 1000000
            value: object.cpuQuota.hasValue ? object.cpuQuota.value : 1000000
            onMoved: object.cpuQuota.value = value
            enabled: object.cpuQuota.hasValue
        }

        Text {
            id: quotaText
            anchors.left: quotaSlider.right
            anchors.verticalCenter: rect.verticalCenter
            text: object.cpuQuota.hasValue ? `${Math.round(object.cpuQuota.value/10000)}%` : "no quota"
            horizontalAlignment: Text.AlignRight
            width: 70
        }

        Button {
            id: killButton
            anchors.left: quotaText.right
            anchors.verticalCenter: rect.verticalCenter
            anchors.leftMargin: 20
            onClicked: object.stop()
            text: "stop"
            width: 50
        }

        Text {
            id: displayText
            anchors.left: killButton.right
            anchors.top: rect.top
            anchors.topMargin: 10
            anchors.leftMargin: 20
            text: display
        }

        Text {
            id: desktopNameText
            anchors.left: killButton.right
            anchors.bottom: rect.bottom
            anchors.bottomMargin: 10
            anchors.leftMargin: 20
            text: object.desktopName
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: instanceText
            anchors.left: displayText.right
            anchors.right: rect.right
            anchors.bottom: rect.bottom
            anchors.bottomMargin: 10
            anchors.rightMargin: 20
            text: object.instance
            horizontalAlignment: Text.AlignRight
            font.family: "monospace"
        }
    }
}
