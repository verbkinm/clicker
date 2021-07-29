#ifndef MAIN_TAB_H
#define MAIN_TAB_H

#include <QWidget>

namespace Ui {
class Main_Tab;
}

class Main_Tab : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Tab(QWidget *parent = nullptr);
    ~Main_Tab();

private slots:
    void on_start_clicked();

signals:
    void signalStart();

private:
    Ui::Main_Tab *ui;
};

#endif // MAIN_TAB_H
