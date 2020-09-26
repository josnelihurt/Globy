import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root
    property int defaultItemHeight : 50
    property string state: 'Off'
    property string address : ''
    signal ledClick();
    signal backClick();
    x: 0
    y: 0
    Grid {
        id: grid
        anchors.fill: parent
        columns: 1
        spacing: 2

        Rectangle {
            width: parent.width
            height: defaultItemHeight
            Text {
                text: "set the globy name"
                anchors.centerIn: parent
            }
            border.color: "black"
            radius: 5
        }
        Rectangle {
            width: parent.width
            height: defaultItemHeight
            Text {
                text: "set the globy state"
                anchors.centerIn: parent
            }
        }
        Rectangle {
            Button {
                anchors.fill: parent
                text: "Send command"
                onClicked: root.ledClick()
            }
            width: parent.width
            height: defaultItemHeight
        }
        Rectangle {
            Button {
                anchors.fill: parent
                text: "Back"
                onClicked: root.backClick()
            }
            width: parent.width
            height: defaultItemHeight
        }
    }

}
