import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Rectangle {
    property alias title: rootTitle.text
    property string fontFamily: "Microsoft JhengHei"
    property int fontPixelSize: 20
    property alias driverX64Text: rootDiverX64.text
    property alias driverX86Text: rootDiverX86.text
    property alias usesWin7Install: chkWin7Install.checked
    property bool numberOnly: false

    signal driverX64EditFinished()
    signal driverX86EditFinished()
    signal win7InstallerChecked()

    border.color: "darkgray"
    border.width: 2
    color: "white"
    radius: 10

    width: 1100
    height: 50

    id: root
    Row{
        spacing: 10
        anchors.centerIn: parent
        Text{
            id: rootTitle
            anchors{
                verticalCenter: rootDiverX64.verticalCenter
            }
            font{
                family: fontFamily
                pixelSize: fontPixelSize
            }
        }

        TextField{
            id: rootDiverX64
            width: 320
            font{
                family: fontFamily
                pixelSize: fontPixelSize
            }
            placeholderText: "X64DirverName(.zip);xxx(.inf),xxx(.inf)"
            selectByMouse: true
            onTextChanged: {
                if (numberOnly){
                    if (Number(text))
                        text = text
                    else
                        text = ""
                }
                else
                    text = text
            }
            onEditingFinished: {
                root.driverX64EditFinished()
            }
        }

        TextField{
            id: rootDiverX86
            width: 320
            font{
                family: fontFamily
                pixelSize: fontPixelSize
            }
            placeholderText: "X86DirverName(.zip);xxx(.inf),xxx(.inf)"
            selectByMouse: true
            onTextChanged: {
                if (numberOnly){
                    if (Number(text))
                        text = text
                    else
                        text = ""
                }
                else
                    text = text
            }
            onEditingFinished: {
                root.driverX86EditFinished()
            }
        }

        CheckBox{
            id: chkWin7Install
            text: "使用win7DriverInstall進行安裝"
            font{
                family: fontFamily
                pixelSize: fontPixelSize
            }
            onCheckStateChanged: {
                root.win7InstallerChecked()
            }
        }
    }//ROW

}
