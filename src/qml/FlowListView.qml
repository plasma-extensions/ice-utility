/*
 *  Copyright 2012 Ruediger Gad
 *
 *  This file is part of FlowListView.
 *
 *  FlowListView is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License (LGPL)
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  FlowListView is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with FlowListView.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.0

Flickable {
    id: flowListView
    anchors.fill: parent

    contentWidth: parent.width;
    contentHeight: flow.childrenRect.height

    property alias count: repeater.count
    property int currentIndex: -1
    property variant currentItem;
    property alias delegate: repeater.delegate
    property alias flow: flow.flow
    property alias model: repeater.model
    property alias spacing: flow.spacing
    property var highlight;
    property bool highlightFollowsCurrentItem: false;
    property var highlightItem;

    onCurrentIndexChanged: {
        // currentItem = model.get(flowListView.currentIndex)
        currentItem = repeater.itemAt(flowListView.currentIndex)

        if (flowListView.highlightFollowsCurrentItem && flowListView.highlight)
            refreshHighlightItemPosition(flowListView.currentIndex)
    }

    function refreshHighlightItemPosition(index) {
        var uiItem = repeater.itemAt(index);

        if (!uiItem)
            highlightItem.visible = false;
        else
            highlightItem.visible = true;

        highlightItem.x = Qt.binding(function () {return uiItem.x});
        highlightItem.y = Qt.binding(function () {return uiItem.y});
        highlightItem.width = Qt.binding(function () {return uiItem.width;});
        highlightItem.height = Qt.binding(function () {return uiItem.height});
    }

    Item {
        id: listContainer;
        anchors.fill: parent;
        Flow {
            id: flow
            width: parent.width

            Repeater {
                id: repeater

                onCountChanged: {
                    if (flowListView.currentIndex === -1 && count > 0) {
                        flowListView.currentIndex = 0
                    }
                    if (flowListView.currentIndex >= count) {
                        flowListView.currentIndex = count - 1
                    }
                }
            }

        }

        Component.onCompleted: {
            highlightItem = highlight.createObject(listContainer, {"layer.enabled": false, z: -100})
            refreshHighlightItemPosition(flowListView.currentIndex)
        }
    }
}
