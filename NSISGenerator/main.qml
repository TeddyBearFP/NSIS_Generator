import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import "views/controls"
import Generator 1.0

Window {
    visible: true
    width: 1700
    height: 800//Screen.desktopAvailableHeight
    minimumWidth: width
    minimumHeight: height
    title: qsTr("NSIS Generator")

    property int fontPixelSize: 20
    color: "white"
    Generator{
        id: generator
        Row{
            anchors{
                top: parent.top
                topMargin: 10
                left: parent.left
                leftMargin: 10
            }
            spacing: 10
            Column{
                spacing: 10
                RowText{
                    title: "Name"
                    inputPlaceholderText: "Program Name"
                    onInputEditFinished: {
                        generator.programName = inputStr
                        generator.setDBProgramName(inputStr)
                    }
                    Component.onCompleted: inputStr = generator.programName
                }
                RowText{
                    title: "Version"
                    inputPlaceholderText: "Program Version"
                    onInputEditFinished: {
                        generator.version = inputStr
                        generator.setDBVersion(inputStr)
                    }
                    Component.onCompleted: inputStr = generator.version
                }
                RowText{
                    title: "Release File"
                    inputPlaceholderText: "Release File Name (.zip)"
                    onInputEditFinished: {
                        generator.mainFile = inputStr
                        generator.setDBProgramReleaseFile(inputStr)
                    }
                    Component.onCompleted: inputStr = generator.mainFile
                }
                RowText{
                    title: "Company Name"
                    inputPlaceholderText: "Company Name"
                    onInputEditFinished: {
                        generator.company = inputStr
                        generator.setDBCompany(inputStr)
                    }
                    Component.onCompleted: inputStr = generator.company
                }
                RowText{
                    title: "icon Name"
                    inputPlaceholderText: "icon Name (.ico需存在release.zip)"
                    onInputEditFinished: {
                        generator.iconName = inputStr
                        generator.setDBIcon(inputStr)
                    }
                    Component.onCompleted: inputStr = generator.iconName
                }
                RowText{
                    id: howManyDrivers
                    title: "How Many Drivers"
                    inputPlaceholderText: "How Many (0~10)"
                    numberOnly: true
                    inputStr: generator.howManyDrivers
                    onInputEditFinished: {
                        if (inputStr !== "")
                            generator.howManyDrivers = inputStr
                    }
                }
                Rectangle{
                    border.color: "darkgray"
                    border.width: 2
                    color: "white"
                    radius: 10
                    width: 220
                    height: checkBoxRepeater.count * 48
                    Column{
                        anchors.centerIn: parent
                        spacing: 5
                        Repeater{
                            id: checkBoxRepeater
                            model:["welcome page",
                                   "Directory page",
                                   "Instfiles page",
                                   "Finish page",
                                   "vcredist_x86",
                                   "功能-開機自動啟動",
                                   "功能-詢問是否重啟",
                                   "功能-是否最高權限"]
                            CheckBox{
                                font{
                                    pixelSize: fontPixelSize
                                }
                                text: modelData
                                checked: true
                                onCheckedChanged: {
                                    generator.setNsisPage(index,checked)
                                }
                            }
                        }

                    }
                }
            }//Column
            Column{
                spacing: 10
                Repeater{
                    id: driversRepeater
                    model: howManyDrivers.inputStr
                    DriverRow{
                        title: "Driver " + (index + 1)
                        onDriverX64EditFinished: {
                            generator.driverX64List[index] = driverX64Text
                        }
                        onDriverX86EditFinished: {
                            generator.driverX86List[index] = driverX86Text
                        }
                        onUsesWin7InstallChanged: {
                            generator.usesWin7Installer[index] = usesWin7Install
                        }
                    }
                }
            }//Column
        } //Row

    }
    Button{
        width: 200
        height: 100
        text: "Generate NSIS"
        anchors{
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
            bottomMargin: 10
        }
        font{
            pixelSize: fontPixelSize
            bold: true
        }
        onClicked: {
            msgBox.title = "NSIS Generator"
            if (generator.makeNsis()==="OK")
                msgBox.text = "產生完成!!"
            else
                msgBox.text = "產生失敗!!"
            msgBox.open()
        }
    }

    MessageDialog{
        id: msgBox
        standardButtons :MessageDialog.Ok
        icon:StandardIcon.Information
    }
}
