# udpTcpSender
-----------------------------------------------------
Основная задача это легкая замена UDP и TCP сокета.

+ вывод в лог отправляемых и принимаемых данных, вывод ошибок
+ индикаторы статуса соединения и отправки датаграммы
+ отправка данных формата: 
	- 16-битные слова;
	- json;
	- обычный текст.
+ настройки соединения применяются после их окончания изменения
-----------------------------------------------------

Пример использования UDP сокета:
```c++
#include "ethernet.h"
#include "udp.h"
void main(){
	QScopedPointer<ethernet::Ethernet> mEthernet;
	mEthernet.reset( new ethernet::Udp() );
	/*
	connect(mEthernet.data(),SIGNAL(signalReadData(int)),SLOT(slotReadData(int)));
	connect(mEthernet.data(),SIGNAL(signalConnectStatus(bool)),mWidgetSettings,SLOT(slotSettingsIsAccept(bool)));
	connect(mEthernet.data(),SIGNAL(signalErrorMsg(QString)),mWidgetLog,SLOT(slotWriteInfo(QString)));
	*/
	mEthernet.data()->setSettings(QHostAddress("127.0.0.1"), 2000, 2000);
}
```
----------------------

Пример использования TCP клиента:
```c++
#include "ethernet.h"
#include "tcpclient.h"
void main(){
	QScopedPointer<ethernet::Ethernet> mEthernet;
	mEthernet.reset( new ethernet::TcpClient() );
	/*
	connect(mEthernet.data(),SIGNAL(signalReadData(int)),SLOT(slotReadData(int)));
	connect(mEthernet.data(),SIGNAL(signalConnectStatus(bool)),mWidgetSettings,SLOT(slotSettingsIsAccept(bool)));
	connect(mEthernet.data(),SIGNAL(signalErrorMsg(QString)),mWidgetLog,SLOT(slotWriteInfo(QString)));
	*/
	mEthernet.data()->setSettings(QHostAddress("127.0.0.1"), 2000);
}
```
----------------------

Пример использования TCP сервера:
```c++
#include "ethernet.h"
#include "tcpserver.h"
void main(){
	QScopedPointer<ethernet::Ethernet> mEthernet;
	mEthernet.reset( new ethernet::TcpServer() );
	/*
	connect(mEthernet.data(),SIGNAL(signalReadData(int)),SLOT(slotReadData(int)));
	connect(mEthernet.data(),SIGNAL(signalConnectStatus(bool)),mWidgetSettings,SLOT(slotSettingsIsAccept(bool)));
	connect(mEthernet.data(),SIGNAL(signalErrorMsg(QString)),mWidgetLog,SLOT(slotWriteInfo(QString)));
	*/
	mEthernet.data()->setSettings(QHostAddress("127.0.0.1"), 2000);
}
```
----------------------

TODO:
1) добавить функторы при отправке данных и реализовать ответ на отправляемую датаграмму туда откуда пришли данные
2) доработать дескрипторы в TCP, делать ответ по конкретному дескриптору (клиенту) и отладить работу TCP
3) сделать вывод в лог текущего времени датаграммы/ошибки
4) сохранение выбранных настроек через QSettings
5) сделать кнопку принудительной установки соединения, а не делать это по окончании изменения данных
6) добавить вывод версии программы в заголовке окна программы
7) проработать вариант pointer в qMap
8) проработать вариант со сравниванием объектов производного класса
