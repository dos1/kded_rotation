#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOrientationSensor>
#include <QTimer>
#include <QProcess>
#include <KDEDModule>
#include <KPluginFactory>

class ScreenRotator : public KDEDModule
{
		Q_OBJECT
		Q_CLASSINFO("D-Bus Interface", "org.kde.rotation")

	public:
		ScreenRotator(QObject *parent, const QVariantList &);
		void updateOrientation();
		void startProgress();
		void updateProgress();
		~ScreenRotator();

	private:
		QOrientationSensor *sensor;
		int progress;
		QTimer timer;
		QOrientationReading::Orientation currentOrientation;
		QProcess helper;
};

#endif // MAINWINDOW_H
