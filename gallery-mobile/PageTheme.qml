import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import "."

Page {
    property alias toolbarButtons: buttonsLoader.sourceComponent
    property alias toolbarTitle: titleLabel.text

    header: ToolBarTheme{
        RowLayout{
            anchors.fill: parent
            ToolButton{
                background: Image{
                    source: Qt.resolvedUrl("qrc:/icons/res/icons/back.svg")
                }
                onClicked: {
                    if(StackView.depth > 1){
                        StackView.pop()
                    }
                }
            }
            Label {
                id: titleLabel
                Layout.fillWidth: true
                color: Style.text
                elide: Text.ElideRight
                font.pointSize: 30
            }
            Loader {
                Layout.alignment: Qt.AlignRight
                id: buttonsLoader
            }
        }
    }
    Rectangle{
        color: Style.pageBackground
        anchors.fill: parent
    }
}
