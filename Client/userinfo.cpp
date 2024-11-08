#include "userinfo.h"

#include <QDebug>
userInfo::userInfo() {}
userInfo::userInfo(QString name , QString passWord,int money,QString account,QString photo):
    name(name),passWord(passWord),money(money),accountType(account),photoNum(photo)
{

}
userInfo::userInfo(QString data){
    QStringList ss = data.split(',');
    if(ss.size() != 5){
        qDebug() << "Error : data is illegal";
    }
    this->name = ss[0];
    this->passWord = ss[1];
    this->money = ss[2].toInt();
    this->accountType = ss[3];
    this->photoNum = ss[4];
}
QString userInfo::serialize(){
    QString res = this->name;
    res.append(',').append(this->passWord);
    res.append(',').append(QString::number(this->money));
    res.append(',').append(this->accountType);
    res.append(',').append(this->photoNum);
    qDebug() << res;
    return res;
}
