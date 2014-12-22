#include "workspace.h"
#include "mainwindow.h"

Workspace::Workspace(MainWindow *mainWindow, QWidget *parent) :
    QScrollArea(parent), m_modified(false), m_mainWindow(mainWindow)
{
    m_canvas = new Canvas(mainWindow);

    this->setBackgroundRole(QPalette::Shadow);
    this->setWidget(m_canvas);
    this->setAlignment(Qt::AlignCenter);
    this->setTitle("Sans nom");
//    this->updateCursor();
    this->setMouseTracking(true);
}

void Workspace::setTitle(const QString title)
{
    m_title = title;
    this->updateWindowTitle();
}

void Workspace::setModified(bool modified)
{
    m_modified = modified;
    this->updateWindowTitle();
}

void Workspace::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    m_canvas->zoom(m_canvas->getZoom() + numSteps);

    event->ignore();
}

void Workspace::keyPressEvent(QKeyEvent *event)
{
    Qt::Key key = this->getKey(event);

    m_keysDown.insert(key);
    m_mainWindow->getTool()->keyPressEvent(key);

    this->updateCursor();
}

void Workspace::keyReleaseEvent(QKeyEvent *event)
{
    Qt::Key key = this->getKey(event);

    m_keysDown.remove(key);
    m_mainWindow->getTool()->keyReleaseEvent(key);

    this->updateCursor();
}

void Workspace::mouseMoveEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);
}

void Workspace::mousePressEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);
}

void Workspace::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouse.handleEvent(event);
}

void Workspace::closeEvent(QCloseEvent *event)
{
    if(m_modified){
        QMessageBox::StandardButton choice = QMessageBox::warning(this, 0, "Save your changes to before leaving ?",
                                                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

        if(choice == QMessageBox::Cancel)
            event->ignore();
        else if (choice == QMessageBox::Yes){
            if(false == this->save())
                event->ignore();
        }
    }
}

void Workspace::updateWindowTitle()
{
    QString marker = m_modified ? "*" : "";

    this->setWindowTitle(m_title + marker);
}

void Workspace::updateCursor()
{
    if(m_keysDown.contains(Qt::Key_Space)){
        if(m_mouse.isButtonDown(Qt::LeftButton))
            this->setCursor(QCursor(Qt::ClosedHandCursor));
        else
            this->setCursor(QCursor(Qt::OpenHandCursor));
    }
    else{
        this->setCursor(*m_mainWindow->getTool()->getCursor());
    }
}

Qt::Key Workspace::getKey(QKeyEvent *event)
{
    if(event->isAutoRepeat())
        return Qt::Key_unknown;

    return static_cast<Qt::Key>( event->key() );
}

bool Workspace::save()
{
    if(m_file.isEmpty())
        return this->saveAs();

    return true;
}

bool Workspace::saveAs()
{
    QSettings settings("crusher.ini", QSettings::IniFormat);

    QString fileName = QFileDialog::getSaveFileName(this, "Save", settings.value("path/save").toString() + "/" + m_title, "Crusher Document (*.cd);;Image (*.png *.jpg)");

    if(fileName == "")
        return false;

    QFileInfo fileInfo(fileName);
    settings.setValue("path/save", fileInfo.absoluteDir().absolutePath());
    this->setTitle(fileInfo.baseName());

    if(fileInfo.suffix() == "cd"){
        m_file = fileInfo.absoluteFilePath();
        this->setModified(false);
    }

    return true;
}
