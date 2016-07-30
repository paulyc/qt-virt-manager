#include "vm_viewer_only.h"

VM_Viewer_Only::VM_Viewer_Only(
        QWidget          *parent,
        const QString     url) :
    QMainWindow(parent), url(url)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(100, 100);
    setContentsMargins(0,0,0,0);
    setWindowTitle(QString("Qt Remote Viewer -- %1").arg(url));
    QIcon::setThemeName("QtRemoteViewer");
    setWindowIcon(QIcon::fromTheme("remote-desktop-viewer"));

    viewerToolBar = new ViewerToolBar(this);
    viewerToolBar->hide();
    addToolBar(Qt::TopToolBarArea, viewerToolBar);
    connect(viewerToolBar, SIGNAL(execMethod(const Act_Param&)),
            this, SLOT(resendExecMethod(const Act_Param&)));

    toolBarPoint = QPoint(0,0);
    animatedShowToolBar = new QPropertyAnimation(
                viewerToolBar, "pos");
    animatedShowToolBar->setDuration(333);
    animatedHideToolBar = new QPropertyAnimation(
                viewerToolBar, "pos");
    animatedHideToolBar->setDuration(333);
    connect(animatedShowToolBar, SIGNAL(finished()),
            this, SLOT(toolBarShowed()));
    connect(animatedHideToolBar, SIGNAL(finished()),
            this, SLOT(hideToolBar()));
    connect(viewerToolBar, SIGNAL(positionChanged(const QPoint&)),
            this, SLOT(setNewPosition(const QPoint&)));
}
VM_Viewer_Only::~VM_Viewer_Only()
{
    //qDebug()<<"VM_Viewer_Only destroy:";
    if ( timerId>0 ) {
        killTimer(timerId);
        timerId = 0;
    };
    if ( killTimerId>0 ) {
        killTimer(killTimerId);
        killTimerId = 0;
    };
    //qDebug()<<"VM_Viewer_Only destroyed";
}

/* public slots */
void VM_Viewer_Only::resendExecMethod(const Act_Param &params)
{
    if ( params.method==reconnectToVirtDomainMethod ) {
        reconnectToVirtDomain();
    } else if ( params.method==sendKeySeqToVirtDomainMethod ) {
        sendKeySeqToVirtDomain((Qt::Key)params.path.toInt());
    } else if ( params.method==getScreenshotFromVirtDomainMethod ) {
        getScreenshotFromVirtDomain();
    } else if ( params.method==copyFilesToVirtDomainMethod ) {
        copyFilesToVirtDomain();
    } else if ( params.method==cancelCopyFilesToVirtDomainMethod ) {
        cancelCopyFilesToVirtDomain();
    } else if ( params.method==copyToClipboardFromVirtDomainMethod ) {
        copyToClipboardFromVirtDomain();
    } else if ( params.method==pasteClipboardToVirtDomainMethod ) {
        pasteClipboardToVirtDomain();
    } else if ( params.method==fullScreenVirtDomainMethod ) {
        fullScreenVirtDomain();
    } else if ( params.method==scaledScreenVirtDomainMethod ) {
        scaledScreenVirtDomain();
    };

}
void VM_Viewer_Only::startCloseProcess()
{
    //qDebug()<<"startCloseProcess";
    if ( killTimerId==0 ) {
        killTimerId = startTimer(PERIOD);
        if ( nullptr!=statusBar() ) statusBar()->show();
    };
    //qDebug()<<killTimerId<<"killTimer";
}
void VM_Viewer_Only::reconnectToVirtDomain()
{

}
void VM_Viewer_Only::sendKeySeqToVirtDomain(Qt::Key key)
{
    Q_UNUSED(key);
}
void VM_Viewer_Only::getScreenshotFromVirtDomain()
{

}
void VM_Viewer_Only::copyFilesToVirtDomain()
{

}
void VM_Viewer_Only::cancelCopyFilesToVirtDomain()
{

}
void VM_Viewer_Only::copyToClipboardFromVirtDomain()
{

}
void VM_Viewer_Only::pasteClipboardToVirtDomain()
{

}
void VM_Viewer_Only::fullScreenVirtDomain()
{

}
void VM_Viewer_Only::scaledScreenVirtDomain()
{

}
void VM_Viewer_Only::showErrorInfo(QString &_msg)
{
    QIcon _icon = QIcon::fromTheme("face-sad");
    icon = new QLabel(this);
    icon->setPixmap(_icon.pixmap(256));
    msg = new QLabel(this);
    msg->setText(_msg);
    infoLayout = new QVBoxLayout();
    infoLayout->addWidget(icon, 0, Qt::AlignHCenter);
    infoLayout->addWidget(msg);
    infoLayout->addStretch(-1);
    info = new QWidget(this);
    info->setLayout(infoLayout);
    setCentralWidget(info);
}
void VM_Viewer_Only::startAnimatedShow()
{
    if ( viewerToolBar->isVisible() ) return;
    viewerToolBar->blockSignals(true);
    viewerToolBar->setWindowFlags(
                Qt::Window | Qt::FramelessWindowHint);
    viewerToolBar->show();
    if ( toolBarPoint.x()==0 && toolBarPoint.y()==0 ) {
        toolBarPoint =
                QPoint(
                    (this->size().width()
                     -viewerToolBar->size().width()
                     )/2, 0);
    };
    animatedShowToolBar->setStartValue(
                mapToGlobal(QPoint(0,0)));
    animatedShowToolBar->setEndValue(
                mapToGlobal(toolBarPoint));
    animatedShowToolBar->start();
    if ( toolBarTimerId==0 )
        toolBarTimerId = startTimer(15000);
}
void VM_Viewer_Only::startAnimatedHide()
{
    if ( toolBarTimerId>0 ) {
        animatedHideToolBar->setStartValue(
                    viewerToolBar->pos());
        animatedHideToolBar->setEndValue(
                    mapToGlobal(QPoint(0,0)));
        viewerToolBar->blockSignals(true);
        animatedHideToolBar->start();
    };
}

/* private slots */
void VM_Viewer_Only::toolBarShowed()
{
    viewerToolBar->blockSignals(false);
}
void VM_Viewer_Only::hideToolBar()
{
    killTimer(toolBarTimerId);
    toolBarTimerId = 0;
    viewerToolBar->hide();
    viewerToolBar->blockSignals(false);
}
void VM_Viewer_Only::setNewPosition(const QPoint &_pos)
{
    toolBarPoint = mapFromParent(_pos);
}
