#ifndef WIDGET_H
#define WIDGET_H

#include <QTabWidget>

#include <main_tab.h>

class Widget : public QTabWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:    
    Main_Tab _main_tab;

signals:
    void signalStart();

};
#endif // WIDGET_H
