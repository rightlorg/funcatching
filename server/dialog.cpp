#include <QtGui>
#include <QtNetwork>

#include <stdlib.h>

#include "dialog.h"
#include "server.h"

Dialog::Dialog(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel;
    statusLabel->setWordWrap(true);
#ifdef Q_OS_SYMBIAN
    QAction *quitAction = new QAction(tr("Exit"), this);
    quitAction->setSoftKeyRole(QAction::NegativeSoftKey);
    addAction(quitAction);
#else
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
#endif

    if (!server.listen()) {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Fortune Client example now.")
                         .arg(ipAddress).arg(server.serverPort()));

#ifdef Q_OS_SYMBIAN
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    setLayout(mainLayout);
#else
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
#endif
    setWindowTitle(tr("Threaded Fortune Server"));
}
