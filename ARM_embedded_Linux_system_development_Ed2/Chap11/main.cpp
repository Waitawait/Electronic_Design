// main.cpp
#include <qapplication.h>
#include "FindDialog.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);		// ������Ӧ�ó������
  FindDialog *dialog = new FindDialog;	// ����FindDialog����
  dialog->show();	// ��ʾFindDialog
  return app.exec();
}
