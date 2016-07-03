/*
 * My project
 */


import QtQuick 2.0
import QtQuick.Layouts 1.2

import org.kde.plasma.core 2.0 as PlasmaCore

import com.plasma_light.ice_utility 1.0

Item {
    Launchers {
        id: launchers;
    }

    ListView {
        width: 640
        height: 480

        model: launchers;
        delegate: RowLayout{
            PlasmaCore.IconItem {
                source: icon
            }
            Text {
                text: name
            }
        }
    }
}
