/*
 * My project
      */
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import QtWebKit 3.0

import com.plasma_light.ice_utility 1.0

Rectangle {
    width: 824
    height: 530

    color: "#EFF0F1"

    Launchers {
        id: launchers
    }

    Text {
        id: header
        text: i18n("Manage your web  applications launchers")
        font.pointSize: 16

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 30
        anchors.bottomMargin: 19
        anchors.leftMargin: 12
        anchors.rightMargin: 8
    }

    Image {
        id: appIcon
        visible: false

        sourceSize.width: 32
        sourceSize.height: 32

        onStatusChanged: {
            if (appIcon.status === Image.Ready) {
                appIcon.grabToImage(function (result) {
                    if (result.saveToFile(
                                list.currentItem.dataModel.favIconPath))
                        list.currentItem.dataModel.favIcon = list.currentItem.dataModel.favIconPath
                })
            }
        }
    }

    WebView {
        id: webview
        visible: false

        onLoadingChanged: {
            switch (loadRequest.status) {
            case WebView.LoadStartedStatus:
                print("loading content from " + loadRequest.url)
                break
            case WebView.LoadSucceededStatus:
                print("load succeded ")
                if (nameInput.text == "")
                    nameInput.text = webview.title

                appIcon.source = webview.icon
                break
            case WebView.LoadFailedStatus:
                print("load failed " + loadRequest.errorString)
                break
            }
        }
    }

    Rectangle {
        id: details
        color: "transparent"

        border.width: 2
        border.color: "#BABCBE"

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 19
        anchors.leftMargin: 12
        anchors.rightMargin: 8

        height: 180

        GridLayout {
            anchors.fill: parent
            id: itemEdit
            columns: 4
            columnSpacing: 41
            rowSpacing: 12

            Text {
                text: i18n("Application details")
                Layout.columnSpan: 4
                Layout.leftMargin: 8
                Layout.topMargin: 12
            }

            TextField {
                id: urlInput
                Layout.fillWidth: true
                Layout.leftMargin: 8

                placeholderText: i18n("Application url")
                onEditingFinished: webview.url = text
            }

            ComboBox {
                id: runnerInput
                Layout.minimumWidth: 200

                model: [i18n("Select browser to use"), "Firefox", "Chromiun"]
            }

            TextField {
                id: customIconInput
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Layout.rightMargin: 8

                placeholderText: i18n("Icon name or path")
            }

            TextField {
                id: nameInput
                Layout.fillWidth: true
                Layout.leftMargin: 8

                placeholderText: i18n("Application name")
            }

            ComboBox {
                id: categoryInput
                Layout.minimumWidth: 200
                model: [i18n(
                        "Select application category"), "AudioVideo", "Audio", "Video", "Development", "Education", "Game", "Graphics", "Network", "Office", "Since", "Settings", "System", "Utility"]
            }

            Button {
                id: setFavIconButton
                iconName: "edit-undo"
                text: i18n("Set FavIcon")
                enabled: list.currentItem.dataModel.favIcon != ""
                onClicked: setFavIcon()
            }

            Button {
                id: chooseIconButton
                Layout.rightMargin: 8

                iconName: "folder-open"
                text: i18n("Set Custom Icon")
                onClicked: setCustomIcon()
            }

            TextField {
                id: descriptionInput
                Layout.fillWidth: true
                Layout.leftMargin: 8
                Layout.bottomMargin: 12
                Layout.columnSpan: 2

                placeholderText: i18n("Application description")
            }

            Button {
                id: saveButton
                Layout.columnSpan: 2
                Layout.bottomMargin: 12

                text: i18n("Apply")
                iconName: "document-save"

                onClicked: storeEntryData()
            }
        }
    }

    Rectangle {
        id: applicationsList
        color: "transparent"

        height: 202

        border.width: 2
        border.color: "#BABCBE"

        anchors.top: details.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 12
        anchors.rightMargin: 8
        anchors.topMargin: 12
        anchors.bottomMargin: 30

        GridLayout {
            id: actionButtons
            anchors.fill: parent

            columns: 4
            rowSpacing: 12

            Text {
                text: i18n("Application management")
                Layout.fillWidth: true
                Layout.columnSpan: 2

                Layout.leftMargin: 8
                Layout.topMargin: 13
            }

            Button {
                Layout.topMargin: 13
                Layout.rightMargin: 8

                text: "add"
                iconName: "list-add"
                onClicked: {
                    launchers.create("", "", "", "", "", "")
                    list.currentIndex = launchers.count - 1
                }
            }

            Button {
                Layout.topMargin: 13

                text: "remove"
                iconName: "list-remove"
                onClicked: launchers.remove(list.currentIndex)
                Layout.rightMargin: 8
            }

            Rectangle {
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.topMargin: 11
                Layout.bottomMargin: 11

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.columnSpan: 4

                border.width: 1
                border.color: "#BABCBE"

                ScrollView {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.topMargin: 11

                    FlowListView {
                        id: list

                        highlight: highlight
                        highlightFollowsCurrentItem: true
                        spacing: 16

                        model: launchers
                        delegate: itemDeledate
                        onCurrentItemChanged: showEntryData(
                                                  list.currentItem.dataModel)
                    }
                }
            }
        }
    }

    Component {
        id: itemDeledate
        ColumnLayout {
            id: laucherDelegate
            anchors.margins: 4

            property var dataModel
            Component.onCompleted: {
                laucherDelegate.dataModel = Qt.binding(function () {
                    return model
                })
            }

            PlasmaCore.IconItem {
                source: icon == "" ? "preferences-web-browser-shortcuts" : icon
                Layout.topMargin: 11
                implicitHeight: 48
                implicitWidth: 48
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.fillWidth: true
            }
            Text {
                text: name == "" ? "new application" : name
                Layout.topMargin: 5
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.fillWidth: true
            }

            MouseArea {
                anchors.fill: parent
                onClicked: list.currentIndex = index
            }
        }
    }
    Component {
        id: highlight
        Rectangle {
            color: "lightsteelblue"
            radius: 0
            y: list.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }

            Behavior on x {
                NumberAnimation {
                }
            }
            Behavior on y {
                NumberAnimation {
                }
            }
        }
    }

    function showEntryData(data) {
        webview.stop()

        urlInput.text = data.url
        nameInput.text = data.name
        descriptionInput.text = data.comment

        var categoryIdx = categoryInput.model.indexOf(data.categories)
        if (categoryIdx === -1)
            categoryIdx = 0
        categoryInput.currentIndex = categoryIdx

        var runnerIdx = runnerInput.model.indexOf(data.runner)
        if (runnerIdx === -1)
            runnerIdx = 0
        runnerInput.currentIndex = runnerIdx

        customIconInput.text = data.icon

        data.favIconPath = launchers.iconsCacheDirPath + "/"
                + list.currentItem.dataModel.uuid + ".png"
    }

    function storeEntryData() {
        var data = list.currentItem.dataModel

        data.url = urlInput.text
        data.name = nameInput.text
        data.comment = descriptionInput.text

        data.categories = categoryInput.currentText
        data.runner = runnerInput.currentText

        // data.usesFavIcon = !useCustomIcon.checked
        data.icon = customIconInput.text
    }

    function setFavIcon() {
        var data = list.currentItem.dataModel
        customIconInput.text = data.favIconPath
    }

    function setCustomIcon() {
        fileDialog.visible = true
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an icon"
        folder: shortcuts.home
        // modality: Qt.NonModal;
        nameFilters: ["Image files(*.jpg *.png *.svg)"]
        onAccepted: customIconInput.text = fileDialog.fileUrl
    }
}
