#include <QtCore/QCoreApplication>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml>

#include "launchermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName("Ice Utility");
    QCoreApplication::setOrganizationDomain("com.plasma_light.ice_utility");

    qmlRegisterType<LauncherModel>("com.plasma_light.ice_utility", 1, 0, "Launchers");

    QQuickView quickView;
    // Setup view
    quickView.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    quickView.setResizeMode(QQuickView::SizeRootObjectToView);
    quickView.show();

    return app.exec();
}
