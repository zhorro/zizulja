import QtQuick 1.0

Rectangle {
    id: listOfRSSFeed
    width: 360
    height: 40

    property alias feedTitle: podcastTitle.text
    property alias updatingStates: updatingStatusText.text
    property alias iconPath: podcastStateIcon.source

    Image {
        id: podcastStateIcon
        x: 0
        y: 4
        source: "://IconsPack//Feed.svg"
    }

    Text {
        id: podcastTitle
        x: 41
        y: 0
        width: 80
        height: 20
        text: "Unknown"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    Text {
        id: updatingStatusText
        x: 41
        y: 23
        width: 80
        height: 20
        text: "Never updated"
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }
}
