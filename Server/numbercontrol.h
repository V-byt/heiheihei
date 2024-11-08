#ifndef NUMBERCONTROL_H
#define NUMBERCONTROL_H

#include <QMainWindow>
#include <QLabel>
namespace Ui {
class numberControl;
}

class numberControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit numberControl(QWidget *parent = nullptr);
    ~numberControl();

private:
    Ui::numberControl *ui;

};

#endif // NUMBERCONTROL_H
