// qt_widget.cpp
#include <qapplication.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qwidget.h>

class MyWidget : public QWidget	// ����MyWidget��̳���QWidget����
{
public:
        MyWidget(QWidget *parent = 0);	// ����MyWidget��Ĺ��캯��
};

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)	// ����MyWidget�๹�캯��
{
        QSpinBox *agenum_sb = new QSpinBox();	// ����Spin�ؼ�
        agenum_sb->setRange(0, 100);	// ����Spin��ֵ��Χ
        agenum_sb->setValue(0);		// ���ó�ʼ��ֵΪ0

        QSlider *agenum_sl = new QSlider(Qt::Horizontal);	// ����Slider�ؼ�
        agenum_sl->setRange(0, 100);	// ����Slider��ֵ��Χ
       agenum_sl->setValue(0);		// ���ó�ʼ��ֵΪ0

        connect(agenum_sb, SIGNAL(valueChanged(int)), agenum_sl, SLOT(setValue(int)));	// ����Spin�ؼ��޸���ֵ��Ӧ����
        connect(agenum_sl, SIGNAL(valueChanged(int)), agenum_sb, SLOT(setValue(int)));	// ����Slider�ؼ��޸���ֵ��Ӧ����

        QHBoxLayout *layout = new QHBoxLayout;	// �����в��ֵĶ���
        layout->addWidget(agenum_sb);		// ���Spin�ؼ�
        layout->addWidget(agenum_sl);		// ���Slider�ؼ�
        setLayout(layout);	// ����MyWidgetʹ���в���
        setWindowTitle("Enter a number");	// ���ô������
};

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        MyWidget widget;	// ����MyWidgt���͵�����

        widget.show();		// ��ʾ����
        return app.exec();
}
