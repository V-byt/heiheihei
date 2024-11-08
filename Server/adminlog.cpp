#include "adminlog.h"
#include "ui_adminlog.h"
#include "mainWin.h"
#include <QMessageBox>
#include <QDebug>
#include <QFontDatabase>
adminLog::adminLog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::adminLog)
{
    ui->setupUi(this);

    // 加载字体文件
    int id = QFontDatabase::addApplicationFont("://resource//MyFont.ttf");
    //int id2 = QFontDatabase::addApplicationFont("://resource//MyFont2.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(id).at(0);
    //QString fontName2 = QFontDatabase::applicationFontFamilies(id2).at(0);
    // 创建一个QFont对象并设置字体
    QFont font(fontName);
    //QFont font2(fontName2);
    font.setPointSize(28);
    ui->text->setFont(font);
    this->setWindowTitle("管理员登录界面");
    this->setWindowIcon(QIcon("://images//admin.png"));

    ui->name_edit->setPlaceholderText("请输入您的管理员账号");
    ui->pass_edit->setPlaceholderText("请输入您的密码");
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    this->setStyleSheet("QMainWindow { background-image: url(://images//back2.jpg); }");

    ui->log_btn->setStyleSheet(styleSheet_btn);
    ui->exit_btn->setStyleSheet(styleSheet_btn);
    ui->eye->setIcon(QIcon("://images//eye_false.png"));
    ui->eye->setIconSize(QSize(60,50));
    ui->eye->setStyleSheet("QPushButton { background-color: transparent; border: none; }");

    connect(ui->exit_btn,&QPushButton::clicked,this,&adminLog::exit);
    connect(ui->log_btn,&QPushButton::clicked,this,&adminLog::logIn);
    connect(ui->eye,&QPushButton::clicked,this,&adminLog::changeEye);
}
//点击退出按钮后
void adminLog::exit(){
    this->close();
}
//点击登录按钮后
void adminLog::logIn(){
    QString name = ui->name_edit->text();
    QString passWord = ui->pass_edit->text();
    if(name == "admin" && passWord == "54088"){
        QMessageBox::information(this,"通知","恭喜您登录成功");
        mainWin *w = new mainWin();
        this->hide();
        w->show();
    }else{
        QMessageBox::warning(this,"警告","您输入的用户名或密码错误!请重试");
    }
}
//眼睛按钮点击后
void adminLog::changeEye(){
    if(this->canSee){
        canSee = false;
        ui->pass_edit->setEchoMode(QLineEdit::Password);
        ui->eye->setIcon(QIcon("://images//eye_false.png"));
    }else{
        canSee = true;
        ui->eye->setIcon(QIcon("://images//eye_true.png"));
        ui->pass_edit->setEchoMode(QLineEdit::Normal);
    }
    this->repaint();
}
adminLog::~adminLog()
{
    delete ui;
}
