#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QSettings>

#include <widget.h>
#include <one_window_coordinates.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotNewNameTab(int index);

    void on_delTab_triggered();
    void slotDelTab(int index);
    void slotTabChanged(int index);

    void slotStart();

    void on_action_steps_triggered();

    void on_action_area_triggered();

    void on_exit_triggered();

private:
    void checkSettingsFileExist();
    void readSettings();
    void saveSettings();

    Ui::MainWindow *ui;

    Widget _tabWidget;
    QSettings _settings;

protected:
    virtual void closeEvent(QCloseEvent *) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H
