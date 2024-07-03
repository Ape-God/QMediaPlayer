#include "COpenFileButton.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMenu>
#include <QEvent>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>

#define LEFT_BUTTON_WIDTH 145
#define RIGHT_BUTTON_WIDTH 60
#define BUTTON_HEIGHT 40

COpenFileButton::COpenFileButton(QWidget* p) : QWidget(p)
{
	//�����ܿ���
	m_totalWidth = LEFT_BUTTON_WIDTH + RIGHT_BUTTON_WIDTH;

	m_pOpenFileButton = new QPushButton(this);
	m_pOpenFileButton->setText(u8"���ļ�");
	m_pOpenFileButton->setFixedSize(LEFT_BUTTON_WIDTH, BUTTON_HEIGHT);

	QString qssLeft = "QPushButton{"
		"padding-left:30px;"
		"border-top-left-radius:20px;border-bottom-left-radius:20px;"
		"background-color: rgb(55, 55, 55);"
		"color:rgb(255, 255, 255);"
		"text-align:left;"
		"}";

	m_pOpenFileButton->setIcon(QIcon(":/videoWidget/resources/videoWidget/openfile.svg"));
	m_pOpenFileButton->setLayoutDirection(Qt::LeftToRight);
	m_pOpenFileButton->setStyleSheet(qssLeft);

	m_pArrowButton = new QPushButton(this);
	m_pArrowButton->setFixedSize(RIGHT_BUTTON_WIDTH, BUTTON_HEIGHT);

	/*QString button_qss = "QPushButton::menu-indicator:open{"
		"image:url(:/titleBar/resources/titleBar/down_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/titleBar/resources/titleBar/up_arrow.svg);"
		"subcontrol-position:right center;"
		"subcontrol-origin:padding;border:none;}";*/

	QString qssRight = "QPushButton{"
		"background-image: url(:/videoWidget/resources/videoWidget/vline.svg);"
		"background-repeat: no-repeat;"
		"background-origin: padding;"
		"background-position: left;"
		"border-top-right-radius:20px;border-bottom-right-radius:20px;"
		"background-color: rgb(55, 55, 55);}"
		"QPushButton::menu-indicator:open{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"
		"subcontrol-origin:padding;border:none;}"
		"QPushButton::menu-indicator:closed{"
		"image:url(:/videoWidget/resources/videoWidget/down_arrow_16px.svg);"
		"subcontrol-position:right center;right:10px;"  
		"subcontrol-origin:padding;border:none;}";

	m_pArrowButton->setStyleSheet(qssRight);

	QString menu_qss = "QMenu{background-color:rgb(253,253,253);}"
		"QMenu::item{"
		"font:16px;"
		"background-color:rgb(253,253,253);"
		"padding:8px 32px;"
		"margin:0px 8px;"
		"border-bottom:1px solid #DBDBDB;}"
		/*ѡ��������*/
		"QMenu::item:selected{background-color: #FFF8DC;}";

	QMenu* pMenu = new QMenu(this);
	pMenu->setStyleSheet(menu_qss);
	pMenu->setFixedWidth(m_totalWidth);
	QAction* pAc1 = new QAction(u8"open file", this);
	QAction* pAc2 = new QAction(u8"open floder", this);
	pMenu->addAction(pAc1);
	pMenu->addAction(pAc2);
	m_pArrowButton->setMenu(pMenu);
	pMenu->installEventFilter(this);  //�����¼����ˣ����������˵���λ��

	QHBoxLayout* pHLay = new QHBoxLayout(this);
	pHLay->addWidget(m_pOpenFileButton);
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
	//pHLay->setMargin(3);
#else
	pHLay->setMargin(3);
#endif
	pHLay->addWidget(m_pArrowButton);
	pHLay->setContentsMargins(0, 0, 0, 0);

	setLayout(pHLay);

	setFixedSize(m_totalWidth, BUTTON_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);  //���ô��ڱ���͸��, ΪԲ�Ǵ���׼��

	connect(m_pOpenFileButton, &QPushButton::clicked, this, &COpenFileButton::on_openfile);
	connect(pAc1, &QAction::triggered, this, &COpenFileButton::on_openfile);
	connect(pAc2, &QAction::triggered, this, &COpenFileButton::on_openFloder);
}

COpenFileButton::~COpenFileButton()
{
}

void COpenFileButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // �����
	painter.setBrush(QBrush(QColor(55, 55, 55)));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	painter.drawRoundedRect(rect, BUTTON_HEIGHT / 2, BUTTON_HEIGHT / 2);  //���ô���Բ�� 15px

	QWidget::paintEvent(event);
}

bool COpenFileButton::eventFilter(QObject* watched, QEvent* event)
{
	if(m_pArrowButton)
	{
		if (event->type() == QEvent::Show && watched == m_pArrowButton->menu())
		{
			int menuXPos = m_pArrowButton->menu()->pos().x() - LEFT_BUTTON_WIDTH;   //Ĭ������ʾ�ڵ�ǰ��ť�����·�
			int menuWidth = m_pArrowButton->menu()->size().width();
			int buttonWidth = m_pArrowButton->size().width();
			QPoint pos = QPoint(menuXPos, m_pArrowButton->menu()->pos().y() + 2);

			m_pArrowButton->menu()->move(pos);
			return true;
		}
	}

	return false;
}

void COpenFileButton::on_openfile()
{
	QString cfgPath = "HKEY_CURRENT_USER\\Software\\QQMediaPlayerCopy";
	QSettings settings(cfgPath, QSettings::NativeFormat);
	QString lastPath = settings.value("openfile_path").toString();  // ��ע�����ȡ·��
	
	if (lastPath.isEmpty())
	{
		lastPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);  //��ȡĬ�ϵ��ĵ�·�� 
	}

	/*QString fileName = QFileDialog::getOpenFileName(
		this, u8"ѡ��Ҫ���ŵ��ļ�",
		lastPath,
		u8"��Ƶ�ļ� (*.flv *.rmvb *.avi *.mp4);; �����ļ� (*.*);; ");*/

	//����ͬʱ�򿪶���ļ�
	QStringList filelist = QFileDialog::getOpenFileNames(
		this, 
		u8"ѡ��Ҫ���ŵ��ļ�",
		lastPath,
		u8"��Ƶ�ļ� (*.flv *.rmvb *.avi *.mp4);; �����ļ� (*.*);; ");

	/*for (int i = 0; i < flielist.size(); i++)
	{
		QString strName = pathlist[i];
		ui->comboBox->addItem(strName);
	}*/


	if (filelist.isEmpty())
	{
		return;
	}

	int end = filelist[0].lastIndexOf("/");
	QString tmppath = filelist[0].left(end + 1);
	settings.setValue("openfile_path", tmppath);  // ����ǰ�򿪵�·��д�뵽ע���

	emit sig_openfile(filelist);
}

void COpenFileButton::on_openFloder()
{
	QString path = QFileDialog::getExistingDirectory(this, "choose src Directory", "/");

	if (path.isEmpty())
	{
		return;
	}
	
	emit sig_openFloder(path);
}