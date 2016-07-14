import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.2

import org.kde.plasma.core 2.0 as PlasmaCore
import QtWebKit 3.0

Rectangle {
    id:root
    width: 1280
    height: 720

    ScrollView {
        anchors.fill: parent

        WebView {
            id: webview
            url: baseUrl
            anchors.fill: parent
            onNavigationRequested: {
                // detect URL scheme prefix, most likely an external link
                var schemaRE = /^\w+:/
                if (schemaRE.test(request.url)) {
                    request.action = WebView.AcceptRequest
                } else {
                    request.action = WebView.IgnoreRequest
                    // delegate request.url here
                }
            }

            onLoadingChanged: {
                switch (loadRequest.status) {
                case WebView.LoadFailedStatus:
                    statusBox.visible = true
                    retryButton.visible = true
                    msgTitle.text = i18n("Error")
                    msgDetails.text = loadRequest.errorString
                    break
                case WebView.LoadStartedStatus:
                    statusBox.visible = true
                    retryButton.visible = false
                    msgTitle.text = i18n("Loading")
                    msgDetails.text = loadRequest.url
                    break
                case WebView.LoadSucceededStatus:
                    rootWindow.setTitle(webview.title)
                    statusBox.visible = false
                    break
                }
            }
        }
    }

    Rectangle {
        id: statusBox
        anchors.centerIn: parent
        width: 200
        height: 100

        border.width: 2
        border.color: "#BABCBE"

        color: "#EFF0F1"
        radius: 4

         GridLayout  {
            columns: 3
            anchors.fill: parent

            Text {
                id: msgTitle
                font.pointSize: 16
                Layout.columnSpan: 2;
                Layout.leftMargin: 12;
            }

            Button {
                id: retryButton
                iconName: "chronometer-reset"
                onClicked: webview.reload()
                visible: false
            }

            Text {
                id: msgDetails
                font.pointSize: 10
                Layout.columnSpan: 3;
                Layout.leftMargin: 12;
            }


        }
    }
}
