#include "widget.h"

Widget::Widget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);

    this->addTab(&_main_tab, "Главная");
    setWindowTitle("Кликер");

    connect(&_main_tab, &Main_Tab::signalStart, this, &Widget::signalStart);
}

Widget::~Widget()
{

}
