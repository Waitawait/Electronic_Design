// quit.cpp
#include <qapplication.h>
#include <qpushbutton.h>

int main( int argc, char **argv )
{
  QApplication app( argc, argv );		// ����Ӧ�ó������

  QPushButton quitButton( "Quit", 0 );	// ���尴ť
  quitButton.resize( 100, 30 );	// ���ð�ť��С

  QObject::connect(&quitButton, SIGNAL(cliked()), &app, SLOT(quit()));	// ���ð�ť�����¼�������

  quitButton.show();	// ��ʾ��ť
  return app.exec();
}
