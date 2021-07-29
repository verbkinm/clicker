#ifndef ONE_WINDOW_COORDINATES_H
#define ONE_WINDOW_COORDINATES_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QThread>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <w32api.h>
#include <windows.h>

namespace Ui {
class One_Window_Coordinates;
}

class One_Window_Coordinates : public QWidget
{
    Q_OBJECT

public:
    enum class viewMode{STEP = 0, AREA = 1};
    struct Data
    {
        int x1, x2, y1, y2, cycleCount, stepDelay, steps;
        QString fileName;
        viewMode mode;
    };

    explicit One_Window_Coordinates(QWidget *parent = nullptr);
    ~One_Window_Coordinates();

    QPoint getStartCoordinates() const;
    QPoint getEndCoordinates() const;

    Data data() const;
    void setData(const Data &data);

    viewMode mode() const;
    void setMode(viewMode mode);

private:
    Ui::One_Window_Coordinates *ui;

    bool _startSeleted, _endSelected;
    viewMode _currentMode;

    void cliker() const;
    void action(QString &buff,  QClipboard *clipboard, int x, int y);
    void setCoordinates(int x, int y);

    void ctrlC();

    void testStepMode();
    void testAreaMode();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    void slotTest();

private slots:
    void slotStartClick();
    void slotStopClick();

    void slotDefaultCursor();
    void slotSelectFile();
};

#endif // ONE_WINDOW_COORDINATES_H
