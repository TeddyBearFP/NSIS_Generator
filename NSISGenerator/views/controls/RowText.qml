import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Rectangle {
    property alias title: rootTitle.text
    property string fontFamily: "Microsoft JhengHei"
    property int fontPixelSize: 20
    property alias inputStr: rootInput.text
    property alias inputPlaceholderText: rootInput.placeholderText
    property bool numberOnly: false

    signal inputEditFinished()

    border.color: "darkgray"
    border.width: 2
    color: "white"
    radius: 10

    width: 550
    height: 50

    id: root

    Text{
        id: rootTitle
        anchors{
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
        font{
            family: fontFamily
            pixelSize: fontPixelSize
        }
    }

    TextField{
        id: rootInput
        width: 350
        anchors{
            right: parent.right
            rightMargin: 10
            verticalCenter: parent.verticalCenter //垂直置中
        }
        font{
            family: fontFamily
            pixelSize: fontPixelSize
        }
        selectByMouse: true
        onTextChanged: {
            if (numberOnly){
                console.log(text)
                if ( parseInt(text) > -1 && (parseInt(text) < 11))
                    text = text
                else
                    text = ""
            }
            else
                text = text
        }
        onEditingFinished: {
            root.inputEditFinished()
        }
    }
}
