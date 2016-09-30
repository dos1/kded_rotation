#include "screenrotator.h"
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QProcess>
#include <KPluginFactory>

ScreenRotator::ScreenRotator(QObject *parent, const QVariantList &) :
  KDEDModule(parent)
{
	sensor = new QOrientationSensor(this);

	sensor->start();

	connect(sensor, &QOrientationSensor::readingChanged, this, &ScreenRotator::startProgress);
	connect(&timer, &QTimer::timeout, this, &ScreenRotator::updateProgress);

	updateOrientation();

	progress = -1;

}

void ScreenRotator::startProgress() {
	if (progress == -1) {
		timer.start(25);
		progress = 0;
	}
}

void ScreenRotator::updateProgress() {
	if (!sensor->reading()) return;
	if (sensor->reading()->orientation() != currentOrientation) {
		progress++;

		QDBusMessage msg = QDBusMessage::createMethodCall(
		    QStringLiteral("org.kde.plasmashell"),
		    QStringLiteral("/org/kde/osdService"),
		    QStringLiteral("org.kde.osdService"),
		    QStringLiteral("mediaPlayerVolumeChanged")
		);

		msg.setArguments({progress, "screen", "view-refresh"});

		QDBusConnection::sessionBus().asyncCall(msg);

		if (progress == 100) {
			updateOrientation();
			timer.stop();
			progress = -1;
		}

	} else {
		timer.stop();
		progress = -1;
	}
}

void ScreenRotator::updateOrientation() {
	if (!sensor->reading()) return;
	currentOrientation = sensor->reading()->orientation();
	QString o;
	switch (currentOrientation) {
		case QOrientationReading::TopUp:
			o = "normal";
			break;
		case QOrientationReading::TopDown:
			o = "bottom-up";
			break;
		case QOrientationReading::LeftUp:
			o = "left-up";
			break;
		case QOrientationReading::RightUp:
			o = "right-up";
			break;
		default:
			o = "other";
			return;
	}

	helper.start("orientation-helper", {o});

}

ScreenRotator::~ScreenRotator()
{
	delete sensor;
}

K_PLUGIN_FACTORY(ScreenRotatorFactory,
                 registerPlugin<ScreenRotator>();)
#include "screenrotator.moc"
