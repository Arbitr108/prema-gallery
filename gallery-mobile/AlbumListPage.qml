import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

PageTheme {
    toolbarTitle: "Альбомы"
    toolbarButtons: ToolButton{
        background: Image {
                   source: "res/icons/album-add.svg"
               }
               onClicked: {
                   newAlbumDialog.open()
               }
    }

    InputDialog{
        id: newAlbumDialog
        title: "Новый альбом"
        label: "Название альбома"
        hint: "Мой альбом"
        onAccepted: {
            albumModel.addAlbumFromName(editText.text)
        }
    }

    ListView {
        id: albumList
        model: albumModel
        spacing: 5
        anchors.fill: parent

        delegate: Rectangle{
            width: parent.width
            height: 120
            color: "#d0d1d2"
            Text{
                text: name
                font.pointSize: 16
                color: "#000000"
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    albumList.currentIndex = index
                    pictureModel.setAlbumId(id)
                    pageStack.push("qrc:/AlbumPage.qml",
                                   {albumName: name, albumRowIndex: index})
                }
            }
        }
    }
}
