// hello_qt.cpp
#include <qapplication.h>
#include <qpushbutton.h>


int main( int argc, char **argv )
{
    QApplication a( argc, argv );	// ����Ӧ�ö���

    QPushButton hello( "Hello world!", 0 );	// ���尴ť����
    hello.resize( 100, 30 );	// ���ð�ť��С

    a.setMainWidget( &hello );	// ����������ؼ�
    hello.show();	// ��ʾ��ť
    return a.exec();
}
