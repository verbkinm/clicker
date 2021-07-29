#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _settings(QSettings::IniFormat, QSettings::UserScope, "VMS", "Clicker")
{
    ui->setupUi(this);
    setWindowTitle("Кликер");
    ui->delTab->setDisabled(true);

    setCentralWidget(&_tabWidget);

    connect(&_tabWidget, &Widget::tabBarDoubleClicked, this, &MainWindow::slotNewNameTab);
    connect(&_tabWidget, &Widget::tabCloseRequested, this, &MainWindow::slotDelTab);
    connect(&_tabWidget, &Widget::currentChanged, this, &MainWindow::slotTabChanged);

    connect(&_tabWidget, &Widget::signalStart, this, &MainWindow::slotStart);

    checkSettingsFileExist();
    readSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotNewNameTab(int index)
{
    QString newName = QInputDialog::getText(this, "Новое имя вкладки", "Имя:", QLineEdit::Normal, _tabWidget.tabText(index));
    if(newName.isEmpty())
        return;

    _tabWidget.setTabText(index, newName);
}

void MainWindow::on_delTab_triggered()
{
    int index = _tabWidget.currentIndex();

    slotDelTab(index);
}

void MainWindow::slotDelTab(int index)
{
    if(index == 0)
        return;

    _tabWidget.removeTab(index);
}

void MainWindow::slotTabChanged(int index)
{

}

void MainWindow::slotStart()
{
    if(_tabWidget.count() < 2)
        return;

    while (GetAsyncKeyState(VK_ESCAPE) != -32767)
    {
        for(int i = 1; i < _tabWidget.count(); i++)
        {
//            if(GetAsyncKeyState(VK_ESCAPE) == -32767)
//                return;

            One_Window_Coordinates *owc = qobject_cast<One_Window_Coordinates*>(_tabWidget.widget(i));
            owc->slotTest();
        }
    }
}

void MainWindow::checkSettingsFileExist()
{
    if(!QFileInfo::exists(_settings.fileName()))
    {

    }
}

void MainWindow::readSettings()
{
    _settings.beginGroup("Tabs");
    foreach(const auto &tabNumber, _settings.childGroups())
    {
        _settings.beginGroup(tabNumber);

        One_Window_Coordinates *owc = new One_Window_Coordinates(this);
        _tabWidget.setCurrentIndex(_tabWidget.addTab(owc, _settings.value("tabText").toString()));

        One_Window_Coordinates::Data data;

        data.x1 = _settings.value("x1").toInt();
        data.x2 = _settings.value("x2").toInt();
        data.y1 = _settings.value("y1").toInt();
        data.y2 = _settings.value("y2").toInt();

        data.cycleCount = _settings.value("cycleCount").toInt();
        data.stepDelay = _settings.value("stepDelay").toInt();
        data.steps = _settings.value("steps").toInt();
        data.fileName = _settings.value("fileName").toString();
        data.mode = static_cast<One_Window_Coordinates::viewMode>(_settings.value("mode").toInt());

        owc->setData(data);

        _settings.endGroup();
    }
    _settings.endGroup();
}

void MainWindow::saveSettings()
{
    _settings.remove("Tabs");
    _settings.beginGroup("Tabs");
    for(int i = 1; i < _tabWidget.count(); i++)
    {
        _settings.beginGroup(QString::number(i));

        One_Window_Coordinates *owc = qobject_cast<One_Window_Coordinates*>(_tabWidget.widget(i));
        One_Window_Coordinates::Data data = owc->data();

        _settings.setValue("tabText", _tabWidget.tabText(i));
        _settings.setValue("x1", data.x1);
        _settings.setValue("x2", data.x2);
        _settings.setValue("y1", data.y1);
        _settings.setValue("y2", data.y2);
        _settings.setValue("cycleCount", data.cycleCount);
        _settings.setValue("stepDelay", data.stepDelay);
        _settings.setValue("steps", data.steps);
        _settings.setValue("fileName", data.fileName);
        _settings.setValue("mode", static_cast<int>(data.mode));

        _settings.endGroup();
    }
    _settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();

    qApp->exit();
}

void MainWindow::on_action_steps_triggered()
{
    One_Window_Coordinates *owc = new One_Window_Coordinates;
    owc->setMode(One_Window_Coordinates::viewMode::STEP);
    _tabWidget.addTab(owc, "Новая вкладка");
}

void MainWindow::on_action_area_triggered()
{
    One_Window_Coordinates *owc = new One_Window_Coordinates;
    owc->setMode(One_Window_Coordinates::viewMode::AREA);
    _tabWidget.addTab(owc, "Новая вкладка");
}

void MainWindow::on_exit_triggered()
{
    close();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event->key();
}
