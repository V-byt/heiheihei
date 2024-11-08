#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

class userInfo
{
public:
    userInfo();
    userInfo(QString name,QString passWord,int money,QString account,QString photoNum);
    QString name;
    QString passWord;
    int money = 0;
    QString accountType;
    QString photoNum;
    //将信息打包成一个字符串
    QString serialize();
    //将字符串变成对象信息
    userInfo(QString data);
};

#endif // USERINFO_H
