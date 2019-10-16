# udpTcpSender
-----------------------------------------------------
�������� ������ ��� ������ ������ UDP � TCP ������.

+ ����� � ��� ������������ � ����������� ������, ����� ������
+ ���������� ������� ���������� � �������� ����������
+ �������� ������ �������: 
	16-������ �����;
	json;
	������� �����.
+ ��������� ���������� ����������� ����� �� ��������� ���������
-----------------------------------------------------

������ ������������� UDP ������:
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
----------------------

������ ������������� TCP �������:
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
----------------------

������ ������������� TCP �������:
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
----------------------

TODO:
1) �������� �������� ��� �������� ������ � ����������� ����� �� ������������ ���������� ���� ������ ������ ������
2) ���������� ����������� � TCP, ������ ����� �� ����������� ����������� (�������) � �������� ������ TCP
3) ������� ����� � ��� �������� ������� ����������/������
4) ���������� ��������� �������� ����� QSettings
5) ������� ������ �������������� ��������� ����������, � �� ������ ��� �� ��������� ��������� ������
6) �������� ����� ������ ��������� � ��������� ���� ���������
7) ����������� ������� pointer � qMap
8) ����������� ������� �� ������������ �������� ������������ ������
