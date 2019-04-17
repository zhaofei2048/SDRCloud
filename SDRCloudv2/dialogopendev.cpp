#include "dialogopendev.h"
#include <QListWidget>
#include <QListWidgetItem>
#include "rtldevice.h"


DialogOpenDev::DialogOpenDev(SDRDevice *dongle, QWidget *parent)
	: QDialog(parent),
	m_dongle(dongle)
{
	ui.setupUi(this);
	connect(ui.listWidgetDevice, SIGNAL(itemSelectionChanged()), this, SLOT(itSelSlot()));
	connect(ui.btnRefresh, SIGNAL(clicked()), this, SLOT(refreshDevList()));
	// 搜寻RTL设备
	openRTL();
}

DialogOpenDev::~DialogOpenDev()
{
}

void DialogOpenDev::setHint(QString hint)
{
	ui.labHint->setText(hint);
}

void DialogOpenDev::addListItem(QListWidgetItem *item)
{
	ui.listWidgetDevice->addItem(item);
}

void DialogOpenDev::clearAllItem()
{
	ui.listWidgetDevice->clear();
}

QListWidgetItem *DialogOpenDev::getSelectedItem()
{
	return ui.listWidgetDevice->currentItem();
}

void DialogOpenDev::setBtnOKState()
{
	if (getSelectedItem() == nullptr)
	{
		ui.btnOK->setEnabled(false);
	}
	else
	{
		ui.btnOK->setEnabled(true);
	}
}

//	搜寻RTL设备
void DialogOpenDev::openRTL()
{
	quint32 count = 0;
	QListWidgetItem *item = nullptr;
	

	count = m_dongle->getDeviceCount();
	if (count == 0)	// 未找到设备
	{
		setHint(tr("未找到RTL设备，请插入RTL设备后再尝试刷新按钮！"));
		setBtnOKState();
	}
	else // 找到了
	{
		QString devName;
		for (quint32 i = 0; i < count; i++)	// 依次获取设备名称添加到列表中显示
		{
			devName = m_dongle->getDeviceName(i);
			item = new QListWidgetItem(QString::asprintf("index:[%d]=>name:", i) + devName);
			item->setData(Qt::UserRole, QVariant(i));
			addListItem(item);
		}
		setHint(tr("请在列表中选择一个可用的RTL设备然后打开！"));
		setBtnOKState();
	}
}

void DialogOpenDev::refreshDevList()
{
	clearAllItem();
	openRTL();
}

void DialogOpenDev::itSelSlot()
{
	setBtnOKState();
}
