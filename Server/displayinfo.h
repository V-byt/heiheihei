#ifndef DISPLAYINFO_H
#define DISPLAYINFO_H

#include <QMainWindow>
#include "userinfo.h"
namespace Ui {
class displayInfo;
}

class displayInfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit displayInfo(QWidget *parent = nullptr,userInfo* data = nullptr);
    ~displayInfo();

private:
    Ui::displayInfo *ui;
    userInfo* data;
};

#endif // DISPLAYINFO_H
