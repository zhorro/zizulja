import QtQuick 1.0

Item {
    id: listOfRSSItem

    property alias title: podcastTilte.text // Заголовок
    property alias duration: podcastDuration.text // Длительность
    property alias oldovost: eldely.text // Олдовость

    property bool neverPlayed: true // Новье
    property bool downloaded: true // Качаеться или уже скачано
    property bool enqued: true // В очереди на скачивание

    width: 360
    height: 40

    Image {
        id: podcastStateIcon
        x: 0
        y: 4
        source: ":/iconsPack/Audio.svg"
    }

    Text {
        id: podcastTilte
        x: 41
        y: 0
        width: 80
        height: 20
        text: "Podcast title"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    Text {
        id: podcastDuration
        x: 41
        y: 23
        width: 80
        height: 20
        text: "Duration"
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 12
    }

    Text {
        id: eldely
        text: "Inf"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
    }
    states: [
        State {
            name: "state_Downloaded"
            when: !neverPlayed && downloaded && !enqued
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_downloaded.svg"
            }
        },
        State {
            name: "state_Downloaded_new"
            when: neverPlayed && downloaded && !enqued
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_downloaded_new.svg"
            }
        },
        State {
            name: "state_Downloading"
            when: downloaded && enqued
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_downloading.svg"
            }
        },

        State {
            name: "state_suspended"
            when: !downloaded && !enqued
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_suspended.svg"
            }
        },
        State {
            name: "state_queued"
            when: !downloaded && enqued
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_queued.svg"
            }
        },
        State {
            name: "state_new"
            when: neverPlayed && downloaded
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_new.svg"
            }
        },
        State {
            name: "state_failed"
            PropertyChanges {
                target: podcastStateIcon
                source: ":/iconsPack/Audio_failed.svg"
            }
        }
    ]
}
