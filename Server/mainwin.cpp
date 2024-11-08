#include "mainwin.h"
#include "ui_mainwin.h"
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSqlQuery>
#include "top_up.h"
#include "Regi.h"
#include "order.h"
#include <displayinfo.h>
mainWin::mainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWin)
{
    ui->setupUi(this);
    this->setStyleSheet(styleSheet_btn);
    this->setWindowIcon(QIcon("://images//Icon.png"));
    this->setWindowTitle("管理员服务界面");
    m_server = new QTcpServer();
    //初始化数据
    //加载数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");


    if (!db.open()) {
        qDebug() << "Error: Connection with database failed: ";
    }
    QSqlQuery query(db);
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS userInfo ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "name TEXT NOT NULL, "
                               "passWord TEXT NOT NULL, "
                               "money TEXT NOT NULL, "
                               "accountType TEXT NOT NULL, "
                               "photoNum TEXT NOT NULL)";
    query.exec(createTableQuery);
    if (query.exec("SELECT * FROM userInfo")) {
        while (query.next()) {
            userInfo infor;
            infor.name = query.value("name").toString();
            infor.passWord = query.value("passWord").toString();
            infor.money = query.value("money").toInt();
            infor.accountType = query.value("accountType").toString();
            infor.photoNum = query.value("photoNum").toString();
            userData[infor.name] = infor;
        }
    }

    db.close();



    //设置table的信息
    QStandardItemModel *model = new QStandardItemModel(0, 5, this);
    model->setHorizontalHeaderLabels(QStringList() << "机器号" << "用户" << "余额" << "用户类型" << "呼叫情况");
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setStyleSheet("QTableView::item { text-align: center;font-family: '华文楷体'; font-size: 12pt; }");
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section { font-family: Arial; font-size: 16pt; }");
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,100);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);


    m_server->listen(QHostAddress::Any,1314);
    connect(m_server,&QTcpServer::newConnection,this,&mainWin::handleNewConnection);
}
//有新用户链接
void mainWin::handleNewConnection(){
    QTcpSocket * socket = m_server->nextPendingConnection();
    m_clients.append(socket);
    connect(socket,&QTcpSocket::readyRead,this,&mainWin::handleReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&mainWin::handleDisconnected);
}
//登录信息(0) 处理
void mainWin::handleData0(QStringList datas,QTcpSocket* socket){
    QString ID = datas.at(1);
    QString passWord = datas.at(2);
    QString num = datas.at(3);
    if(!userData.contains(ID)){
        socket->write(QString("notRegister").toUtf8());
        return;
    }
    if(rows[num.toInt()] != -1) {
        socket->write(QString("hasPeople").toUtf8());
        return;
    }
    if(!canBorad.contains(ID)){
        socket->write(QString("toUp").toUtf8());
        return;
    }
    if(userData[ID].passWord != passWord){
        socket->write(QString("passWord").toUtf8());
        return;
    }else{
        socket->write(QString("success").toUtf8());
        computerToUser[num] = ID;
        userToComputer[ID] = num;
        idToS[ID] = socket;
        sToId[socket] = ID;
        int row = m_clients.size() - 1;
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
        userInfo u = userData[ID];
        QStandardItem *item = new QStandardItem(num);
        item->setEditable(false); // 禁止编辑
        model->setItem(row, 0, item); // 将item添加到模型的指定位置
        item = new QStandardItem(ID);
        item->setEditable(false); // 禁止编辑
        model->setItem(row, 1, item);
        item = new QStandardItem(QString::number(u.money));
        item->setEditable(false); // 禁止编辑
        model->setItem(row, 2, item);
        item = new QStandardItem(u.accountType);
        item->setEditable(false); // 禁止编辑
        model->setItem(row, 3, item);
        item = new QStandardItem("暂未呼叫");
        item->setEditable(false); // 禁止编辑
        model->setItem(row, 4, item);
        rows[num.toInt()] = row;

    }
}
//处理用户信息索要请求
void mainWin::sendInfoData(QStringList datas,QTcpSocket* socket){
    QString ID = sToId[socket];
    QString text = "information,";
    //qDebug() << text.append(userData[ID].serialize());
    socket->write(text.append(userData[ID].serialize()).toUtf8());
}
//接收到新信息
void mainWin::handleReadyRead(){
    QTcpSocket*socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = socket->readAll();
    QString str_data = QString::fromUtf8(data);
    QStringList list = str_data.split(',');
    if(list[0] == "0"){
        //处理登录时的信息
        handleData0(list,socket);
    }else if(list[0] == "imformation"){
        //处理信息索要请求
        sendInfoData(list,socket);
    }else if(list[0] == "exit"){
        //处理用户下机
        handleUserDown(list,socket);
    }else if(list[0] == "changeInformation"){
        //处理用户信息修改
        handleUserInfoChange(list,socket);
    }else if(list[0] == "call"){
        //处理呼叫网管 call,机号
        whenCalled(list);
    }else if(list[0] == "getVIP"){
        //处理用户办理会员的请求 getVIP,机号
        handleGetVIP(list,socket);
    }else if(list[0] == "logOut"){
        //处理用户注销账户请求
        handleLogOut(list,socket);
    }else if(list[0] == "shop"){
        //处理用户订单
        order *o = new order(this,userToComputer[sToId[socket]],list[1],list[2]
                             ,list[3],list[4],list[5],list[6]
                             );
        o->show();
    }
}
//处理用户注销账户请求
void mainWin::handleLogOut(QStringList datas,QTcpSocket* socket){
    int computer = datas[1].toInt();
    QString ID = computerToUser[QString::number(computer)];
    userData.remove(ID);
    canBorad.remove(ID);
    int row = rows[computer];
    rows[computer] = -1;
    //更新rows
    for(int i = 0;i < rows.size();++i){
        if(rows[i] > row){
            rows[i]--;
        }
    }
    userToComputer.remove(ID);
    computerToUser.remove(QString::number(computer));
    //更新QTableView中的数据
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    model->removeRow(row);
    socket->write(QString("logOut").toUtf8());
    idToS.remove(ID);
    sToId.remove(socket);
}
//处理用户办理会员的请求
void mainWin::handleGetVIP(QStringList datas,QTcpSocket* socket){
    int comNum = datas[1].toInt();
    int row = rows[comNum];
    setTableValue(row,4,QString("VIP"));
    QString ID = computerToUser[QString::number(comNum)];
    this->userData[ID].accountType = "VIP";
    socket->write(QString("VIP").toUtf8());
}
//处理呼叫网管
void mainWin::whenCalled(QStringList data){
    int com = data[1].toInt();
    int row = rows[com];
    setTableValue(row,4,QString("正在呼叫"));
    calles.insert(QString::number(com),1);
}
//处理用户信息修改
void mainWin::handleUserInfoChange(QStringList datas,QTcpSocket* socket){
    userInfo temp(datas[1],datas[2],datas[3].toInt(),datas[4],datas[5]);
    QString ID = temp.name;
    userData[ID] = temp;
}
//处理用户下机
void mainWin::handleUserDown(QStringList datas,QTcpSocket* socket){
    userInfo temp = userInfo(datas[1],datas[2],datas[3].toInt(),datas[4],datas[5]);
    int computer = datas[6].toInt();
    canBorad.remove(temp.name);
    this->userData[temp.name] = temp;
    int row = rows[computer];
    rows[computer] = -1;
    //更新rows
    for(int i = 0;i < rows.size();++i){
        if(rows[i] > row){
            rows[i]--;
        }
    }
    userToComputer.remove(temp.name);
    computerToUser.remove(QString::number(computer));
    //更新QTableView中的数据
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    model->removeRow(row);
    //给客户端发送成功信息
    socket->write(QString("exitSuccess").toUtf8());
    idToS.remove(temp.name);
    sToId.remove(socket);
}
//处理有用户断开链接
void mainWin::handleDisconnected(){
    QTcpSocket*socket = qobject_cast<QTcpSocket*>(sender());
    QString ID = sToId[socket];
    idToS.remove(ID);
    sToId.remove(socket);
    this->m_clients.removeOne(socket);
}
mainWin::~mainWin()
{
    delete ui;
}
//用户注册
void mainWin::on_reg_btn_clicked()
{
    Regi * re = new Regi(&(this->userData),this);
    re->show();
    //qDebug() << this->userData.size();
}

//用户充值
void mainWin::on_topUp_btn_clicked()
{
    top_up * tp = new top_up(&(this->userData),this);
    tp->show();

}
//更新表中的数据
void mainWin::setTableValue(int i,int j,QVariant val){
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView->model());
    model->setData(model->index(i, j), val, Qt::DisplayRole);
}
//更新充值后的数据
void mainWin::updata_top(QString top_id){
    if(!userToComputer.contains(top_id))return;
    int money = userData[top_id].money;
    int n = rows[userToComputer[top_id].toInt()];
    //此处还需要更新tableView中的余额(如果top_id正在上网的话)
    setTableValue(n,3,money);
    //还需要给top_id的客户端发送信息(如果top_id正在上网的话)
    if(idToS.contains(top_id)){
        QTcpSocket *socket = idToS[top_id];
        QString meg = "userTopup," + QString::number(money);
        socket->write(meg.toUtf8());
    }
}
//保存数据
void mainWin::saveData(){
    db.setDatabaseName("database.db");
    if (!db.open()) {
        qDebug() << "Error: Connection with database failed";
        return;
    }

    QSqlQuery query(db);
    QString clearTableSql = "DELETE FROM userInfo;";

    if (!query.exec(clearTableSql)) {
        qDebug() << "Error: Unable to clear the table";
    } else {
        qDebug() << "Table uersInfo cleared successfully";
    }

    for(userInfo info : this->userData){
        //qDebug() << info.money;
        QString sql = QString("INSERT INTO userInfo (name, passWord, money, accountType, photoNum) VALUES ('%1', '%2', '%3', '%4', '%5')")
                          .arg(info.name)
                          .arg(info.passWord)
                          .arg(QString::number(info.money))
                          .arg(info.accountType)
                          .arg(info.photoNum);

        if(!query.exec(sql)){
            qDebug() << "sql insert fial" << query.lastError().text();
        }else{
            //qDebug() << "sql insert success";
        }
    }
    // 关闭数据库连接
    db.close();
}
//退出系统
void mainWin::on_exit_btn_clicked()
{
    // if(m_clients.size() != 0){
    //     QMessageBox::warning(this,"警告","还有用户链接,不可退出!");
    //     return;
    // }
    this->close();
}
void mainWin::closeEvent(QCloseEvent* en){
    if(m_clients.size() != 0){
        QMessageBox::warning(this,"警告","还有用户链接,不可退出!");
        en->ignore();
    }else{
        saveData();
        QMainWindow::closeEvent(en);
    }
}

//用户上机
void mainWin::on_board_btn_clicked()
{
    QString dlgTitle = "输入";
    QString txtLabel = "请输入要上机的身份证号";
    QString defaultInput = ""; // 可以设置默认输入值
    QLineEdit::EchoMode echoMode = QLineEdit::Normal; // 输入框的文本显示模式
    bool ok;
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
    if (ok && !text.isEmpty()) {
        if(!userData.contains(text)){
            QMessageBox::warning(this,"警告","该用户信息还未注册,上机失败!");
        }else{
            QMessageBox::information(this,"提示","用户"+text+"上机成功!");
            canBorad.insert(text,1);
        }
    }
}

//用户注销
void mainWin::on_control_btn_clicked()
{
    QString dlgTitle = "输入";
    QString txtLabel = "请输入要注销的身份证号";
    QString defaultInput = ""; // 可以设置默认输入值
    QLineEdit::EchoMode echoMode = QLineEdit::Normal; // 输入框的文本显示模式
    bool ok;
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
    if (ok && !text.isEmpty()) {
        if(!userData.contains(text)){
            QMessageBox::warning(this,"警告","该用户信息还未注册,注销失败!");
        }else{
            QMessageBox::information(this,"提示","用户"+text+"注销成功!");
            userData.remove(text);
            canBorad.remove(text);
        }
    }
}

//查询信息
void mainWin::on_search_btn_clicked()
{
    QString dlgTitle = "输入";
    QString txtLabel = "请输入要查询账户的身份证号";
    QString defaultInput = ""; // 可以设置默认输入值
    QLineEdit::EchoMode echoMode = QLineEdit::Normal; // 输入框的文本显示模式
    bool ok;
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
    if (ok && !text.isEmpty()) {
        if(!userData.contains(text)){
            QMessageBox::warning(this,"警告","该用户信息还未注册,查询失败!");
        }else{
            displayInfo* w = new displayInfo(this,&userData[text]);
            w->show();
        }
    }
}

//处理已经呼叫的用户
void mainWin::on_query_btn_clicked()
{
    QString dlgTitle = "输入";
    QString txtLabel = "请输入已经处理呼叫的机器号";
    QString defaultInput = ""; // 可以设置默认输入值
    QLineEdit::EchoMode echoMode = QLineEdit::Normal; // 输入框的文本显示模式
    bool ok;
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
    QRegularExpression re("^(1?[0-9]{1,2}|2[0-9]{2}|300)$");
    if(!re.match(text).hasMatch()){
        QMessageBox::warning(this,"警告","输入了非法机号!");
        return;
    }
    if (ok && !text.isEmpty()) {
        int com = text.toInt();
        if(!calles.contains(text)){
            QMessageBox::warning(this,"警告","该机号目前没有呼叫");
        }else{
            setTableValue(rows[com],4,QString("暂未呼叫"));
        }

    }
}

