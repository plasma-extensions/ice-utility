/*
 * My project
 */


import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

import org.kde.plasma.core 2.0 as PlasmaCore

import com.plasma_light.ice_utility 1.0

Rectangle {
    width: 824;
    height: 530;
    color: PlasmaCore.Theme.backgroundColor

    Component.onCompleted: {
        print ("PlasmaCore: ")
        for(var k in PlasmaCore)
            print (k + " : " + PlasmaCore[k])
    }
    Launchers {
        id: launchers;
    }

    Text {
        id: header
        text: i18n("Manage your web  applications")
        font.pointSize: 16

        anchors.top: parent.top;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.topMargin: 30;  anchors.bottomMargin: 19;
        anchors.leftMargin: 12; anchors.rightMargin: 8;
    }

    GroupBox {
        title: i18n("Application details")
        id: details

        anchors.top: header.bottom;
        anchors.left: parent.left; anchors.right: parent.right;
        anchors.leftMargin: 12; anchors.rightMargin: 8;

        GridLayout {
            anchors.fill: parent;
            id: itemEdit;
            columns: 4;
            columnSpacing: 41;
            rowSpacing: 12;


            TextField {id: urlInput; placeholderText: i18n("Application url"); text : model.currentItem.url; Layout.fillWidth : true;}


            ComboBox {
                id: categoryInput;
                Layout.minimumWidth: 200
                model: [ i18n("Select application category"), "Internet", "Multimedia", "Development" ];
            }

            CheckBox {
                    id: useCustomIcon;
                    text: i18n("Use custom icon");
                    Layout.columnSpan: 2;
                }


            TextField {id: nameInput; placeholderText: i18n("Application name"); text : model.currentItem.name; Layout.fillWidth : true}

            ComboBox {
                id: runnerInput;
                Layout.minimumWidth: 200
                model: [ i18n("Select browser to use"), "Firefox", "Chromiun", "QWebKit"];
            }

            RowLayout {
                Layout.columnSpan: 2;
                TextField {id: customIconInput; placeholderText: i18n("Icon name or path"); text : model.currentItem.icon; Layout.fillWidth: true}


                Button {
                    id: chooseIconButton;
                    Layout.leftMargin: 9;
                    Layout.maximumWidth: height;
                    iconName: "folder-open"
                    iconSource: "file:///usr/share/icons/breeze/places/16/folder-open.svg"
                }

            }
            TextField {id: descriptionInput; placeholderText: i18n("Application description"); text : model.currentItem.comment; Layout.fillWidth : true}

        }
    }

    GroupBox {
        id: applicationsList
        title: i18n("Application management")

        anchors.top: details.bottom; anchors.bottom: parent.bottom;
        anchors.left: parent.left; anchors.right: parent.right;
        anchors.leftMargin: 12; anchors.rightMargin: 8;

        RowLayout {
            id: actionButtons

            layoutDirection: Qt.RightToLeft
            anchors.top: details.bottom;
            anchors.right: parent.right;

            Button {
                text: "remove";
                iconName: "list-remove";
                onClicked: launchers.remove(list.currentIndex);
            }

            Button{
                text: "add"; iconName: "list-add";
                onClicked: {
                    launchers.create("new launcher","preferences-web-browser-shortcuts","Internet;","","http://mysiteapp.com","short description");
                    list.currentIndex = launchers.count - 1;
                }
            }


        }

        ScrollView {
            anchors.top: actionButtons.bottom; anchors.bottom: parent.bottom;
            anchors.left: parent.left; anchors.right: parent.right;

            FlowListView {
                id: list;

                anchors.fill: parent
                // highlight: highlight
                // highlightFollowsCurrentItem: true;
                spacing: 16

                model: launchers;
                delegate: ColumnLayout {
                    PlasmaCore.IconItem {
                        source: icon
                        Layout.topMargin: 11;
                        implicitHeight: 48; implicitWidth: 48;
                        Layout.alignment:  Qt.AlignVCenter | Qt.AlignHCenter
                        Layout.fillWidth: true;
                    }
                    Text {
                        text: name
                        Layout.topMargin: 5;
                        Layout.alignment:  Qt.AlignVCenter | Qt.AlignHCenter
                        Layout.fillWidth: true;
                    }

                    MouseArea {
                        anchors.fill: parent;
                        onClicked: list.currentIndex = index;
                    }
                }
            }
        }

    }
    Component {
        id: highlight
        Rectangle {
            width: 180; height: 40
            color: "lightsteelblue"; radius: 5
            y: list.currentItem.y
            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }




}
