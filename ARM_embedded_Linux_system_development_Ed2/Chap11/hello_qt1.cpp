// hello_qt.cpp
#include <qapplication.h>
#include <qlabel.h>

int main( int argc, char **argv )
{
  QApplication a( argc, argv );	// ����Ӧ�ó������
  QLabel hello("<h1><i>Hello,World!</i></h1>", 0);	// �����ǩ

  hello.show();	// ��ʾ��ǩ
  return a.exec();
}
