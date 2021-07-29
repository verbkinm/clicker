#include "one_window_coordinates.h"
#include "ui_one_window_coordinates.h"

One_Window_Coordinates::One_Window_Coordinates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::One_Window_Coordinates), _startSeleted(false), _endSelected(false), _currentMode(viewMode::STEP)
{
    ui->setupUi(this);

    connect(ui->startButton, &QPushButton::pressed, this, &One_Window_Coordinates::slotStartClick);
    connect(ui->endButton, &QPushButton::pressed, this, &One_Window_Coordinates::slotStopClick);
    connect(ui->testButton, &QPushButton::clicked, this, &One_Window_Coordinates::slotTest);

    connect(ui->startButton, &QPushButton::clicked, this, &One_Window_Coordinates::slotDefaultCursor);
    connect(ui->endButton, &QPushButton::clicked, this, &One_Window_Coordinates::slotDefaultCursor);

    connect(ui->selectFile, &QPushButton::clicked, this, &One_Window_Coordinates::slotSelectFile);
}

One_Window_Coordinates::~One_Window_Coordinates()
{
    delete ui;
}

QPoint One_Window_Coordinates::getStartCoordinates() const
{
    return QPoint(ui->startX->text().toInt(), ui->startY->text().toInt());
}

QPoint One_Window_Coordinates::getEndCoordinates() const
{
    return QPoint(ui->endX->text().toInt(), ui->endY->text().toInt());
}

One_Window_Coordinates::Data One_Window_Coordinates::data() const
{
    Data data;
    data.x1 = ui->startX->value();
    data.x2 = ui->endX->value();
    data.y1 = ui->startY->value();
    data.y2 = ui->endY->value();

    data.cycleCount = ui->count->value();
    data.steps = ui->steps->value();
    data.stepDelay = ui->delay->value();
    data.fileName = ui->filePath->text();

    data.mode = _currentMode;

    return data;
}

void One_Window_Coordinates::setData(const Data &data)
{
    ui->startX->setValue(data.x1);
    ui->endX->setValue(data.x2);
    ui->startY->setValue(data.y1);
    ui->endY->setValue(data.y2);

    ui->count->setValue(data.cycleCount);
    ui->steps->setValue(data.steps);
    ui->delay->setValue(data.stepDelay);
    ui->filePath->setText(data.fileName);

    setMode(data.mode);
}

One_Window_Coordinates::viewMode One_Window_Coordinates::mode() const
{
    return _currentMode;
}

void One_Window_Coordinates::setMode(viewMode mode)
{
    if(mode == viewMode::STEP)
    {

    }
    else if(mode == viewMode::AREA)
    {
        ui->label_steps->setVisible(false);
        ui->steps->setVisible(false);
    }

    _currentMode = mode;
}

void One_Window_Coordinates::cliker() const
{
    if(ui->lbm->isChecked())
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
    }
    else if(ui->rbm->isChecked())
    {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, GetMessageExtraInfo());
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, GetMessageExtraInfo());
    }
    else if(ui->dlbm->isChecked())
    {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, GetMessageExtraInfo());
    }
}

void One_Window_Coordinates::action(QString &buff, QClipboard *clipboard, int x, int y)
{
    SetCursorPos(x, y);
    cliker();
    ctrlC();
    buff += clipboard->text();
    QThread::msleep(ui->delay->value());
}

void One_Window_Coordinates::setCoordinates(int x, int y)
{
    if(GetAsyncKeyState(VK_ESCAPE) == -32767)
        qApp->exit();

    if(_startSeleted)
    {
        ui->startX->setValue(x);
        ui->startY->setValue(y);
    }
    else if(_endSelected)
    {
        ui->endX->setValue(x);
        ui->endY->setValue(y);
    }
}

void One_Window_Coordinates::ctrlC()
{
    //    if(GetAsyncKeyState(VK_ESCAPE) == -32767)
    //        return;

    QThread::usleep(1);
    keybd_event (VK_CONTROL, 0, 0, 0);
    keybd_event ('C', 0, 0, 0);
    QThread::usleep(1);
    keybd_event('C', 0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_CONTROL, 0,KEYEVENTF_KEYUP,0);
    QThread::usleep(1);
}

void One_Window_Coordinates::mouseReleaseEvent(QMouseEvent *event)
{
    setCoordinates(event->globalPos().x(), event->globalPos().y());

    setCursor(Qt::ArrowCursor);
    _startSeleted = false;
    _endSelected = false;
}

void One_Window_Coordinates::slotStartClick()
{
    setCursor(Qt::CrossCursor);
    _startSeleted = true;
    _endSelected = false;
}

void One_Window_Coordinates::slotStopClick()
{
    setCursor(Qt::CrossCursor);
    _startSeleted = false;
    _endSelected = true;
}

void One_Window_Coordinates::testStepMode()
{
    QClipboard *clipboard = QApplication::clipboard();

    for (int j = 0; j < ui->count->value(); ++j)
    {
        //        if(GetAsyncKeyState(VK_ESCAPE) == -32767)
        //            return;

        QFile file(ui->filePath->text());
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи: \"" + ui->filePath->text() + "\"");
            return;
        }
        QString buff;

        //        action(buff, clipboard, getStartCoordinates().x(), getStartCoordinates().y());
        action(buff, clipboard, getStartCoordinates().x(), getStartCoordinates().y());

        float part_1 = 1;
        float part_2 = ui->steps->value() - part_1; // 3 - 1 = 2

        float Ax, Bx, Ay, By, Mx, My;
        float lambda;

        Ax = getStartCoordinates().x();
        Ay = getStartCoordinates().y();
        Bx = getEndCoordinates().x();
        By = getEndCoordinates().y();

        for (int i = 0; i < ui->steps->value() - 1; ++i)
        {
            lambda = part_1 / part_2;

            Mx = (Ax + lambda * Bx) / (1 + lambda);
            My = (Ay + lambda * By) / (1 + lambda);

            part_1++;
            part_2--;
            /*
            if(GetAsyncKeyState(VK_ESCAPE) == -32767)
                return*/;

            action(buff, clipboard, Mx, My);
        }
        action(buff, clipboard, getEndCoordinates().x(), getEndCoordinates().y());

        file.write(buff.toStdString().c_str());
        file.close();
    }
}

void One_Window_Coordinates::testAreaMode()
{
    QClipboard *clipboard = QApplication::clipboard();

    for (int j = 0; j < ui->count->value(); ++j)
    {
        //        if(GetAsyncKeyState(VK_ESCAPE) == -32767)
        //            return;

        QFile file(ui->filePath->text());
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи: \"" + ui->filePath->text() + "\"");
            return;
        }

        SetCursorPos(getStartCoordinates().x(), getStartCoordinates().y());
        cliker();
        QThread::msleep(ui->delay->value());
        keybd_event (VK_LSHIFT, 0, 0, 0);
        SetCursorPos(getEndCoordinates().x(), getEndCoordinates().y());
        cliker();
        QThread::msleep(ui->delay->value());
        keybd_event(VK_LSHIFT, 0,KEYEVENTF_KEYUP,0);
        ctrlC();

        file.write(clipboard->text().toStdString().c_str());
        file.close();
    }
}

void One_Window_Coordinates::slotDefaultCursor()
{
    setCursor(Qt::ArrowCursor);
    _startSeleted = false;
    _endSelected = false;
}

void One_Window_Coordinates::slotSelectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString::fromUtf8("Открыть файл"),
                                                    QDir::currentPath());

    if(fileName.isEmpty())
        return;

    ui->filePath->setText(fileName);
}

void One_Window_Coordinates::mouseMoveEvent(QMouseEvent *event)
{
    setCoordinates(event->globalPos().x(), event->globalPos().y());
}

void One_Window_Coordinates::slotTest()
{
    if(_currentMode == viewMode::STEP)
        testStepMode();
    else if(_currentMode == viewMode::AREA)
        testAreaMode();
}
