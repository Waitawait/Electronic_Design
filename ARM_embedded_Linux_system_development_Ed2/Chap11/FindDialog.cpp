// FindDialog.cpp
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "FindDialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent)		// ���캯��
{
  Label = new QLabel(tr("Find &String:"), this);	// �����ı���ǩ�ؼ�
  LineEdit = new QLineEdit(this);	// �����ı���ؼ�
  Label->setBuddy(LineEdit);	// ���ı���ؼ��ͱ�ǩ�ؼ�

  CaseCB = new QCheckBox(tr("Match &Case"), this);	// ������СдCheckBox
  BackwardCB = new QCheckBox(tr("Search &backward"), this);	// ������������CheckBox

  FindBtn = new QPushButton(tr("&Find"), this);	// �������Ұ�ť
  FindBtn->setDefault(true);	// ���ò��Ұ�ťΪ����״̬

  CloseBtn = new QPushButton(tr("Close"), this);	// �����رհ�ť
  connect(LineEdit, SIGNAL(textChanged(const QString&)), this,
     SLOT(enableFindButton(const QString &)));	// �����޸��ı����¼���Ӧ����
  connect(FindBtn, SIGNAL(clicked()), this, SLOT(findClicked()));	// ���õ������Ұ�ť��Ӧ����
  connect(CloseBtn, SIGNAL(clicked()), this, SLOT(close()));		// ���õ����رհ�ť��Ӧ����

  QHBoxLayout *TopLeft = new QHBoxLayout;		// �����ж���Ĳ��ֶ���
  TopLeft->addWidget(Label);		// ����ı���ǩ�ؼ����ж��벼��
  TopLeft->addWidget(LineEdit);	// ����ı���ؼ����ж��벼��

  QVBoxLayout *Left = new QVBoxLayout;	// �����ж���Ĳ��ֶ���
  Left->addLayout(TopLeft);		// ����ж��벼�ֵ��ж��벼��
  Left->addWidget(CaseCB);		// ��Ӵ�Сд��ѡCheckBox�ؼ����в���
  Left->addWidget(BackwardCB);	// ���ǰ��������CheckBox�ؼ����в���

  QVBoxLayout *Right = new QVBoxLayout;	// �����Ҷ�����в��ֶ���
  Right->addWidget(FindBtn);		// ��Ӳ��Ҷ��󲼾ֵ��Ҷ��벼��
  Right->addWidget(CloseBtn);		// ��ӹرհ�ť���Ҷ��벼��
  Right->addStretch(1);

  QHBoxLayout *Main = new QHBoxLayout(this);	// ���������е������ֶ���
  Main->setMargin(11);
  Main->setSpacing(4);		// ���ÿؼ����վ���
  Main->addLayout(Left);		// �������벼��
 Main->addLayout(Right);		// ����Ҷ��벼��
  setLayout(Main);			// ����Ӧ�ó���ʹ��������

  setWindowTitle(tr("Find Dialog"));	// ���ô������

}

void FindDialog::FindClicked()	// ���Ұ�ť��Ӧ����
{
  QString text = LineEdit->text();	// �Ӳ����ı����ȡҪ���ҵ��ı�
  bool CaseSensitive = CaseCB->isChecked();		// ��ȡ�Ƿ���Ҫ��Сд����

  if (BackwardCB->isChecked())	// �ж���ǰ�����������
    FindPrev(text, CaseSensitive);	// ��ǰ�����ı�
  else
    FindNext(text, CaseSensitive);	// ��������ı�
}

void FindDialog::EnableFindButton(const QString &Text)	// ����������ť
{
}
