#include "qtapplication.h"
#include <pcl/point_cloud.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>


QtApplication::QtApplication(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	initial();//��ʼ��	
	
	//***** �˵����͹������źźͲۺ������� *****/
	// �ļ�(connect)
	QObject::connect(ui.action_Open, &QAction::triggered, this, &QtApplication::Open);	
	QObject::connect(ui.action_Add, &QAction::triggered, this, &QtApplication::Add);
	QObject::connect(ui.action_Clear, &QAction::triggered, this, &QtApplication::Clear);	
	QObject::connect(ui.action_Save, &QAction::triggered, this, &QtApplication::Save);
	//QObject::connect(ui.action_SaveAs, &QAction::triggered, this, &QtApplication::SaveAsBinary);
	QObject::connect(ui.action_Exit, &QAction::triggered, this, &QtApplication::Exit);
	// ��ʾ(connect)
	QObject::connect(ui.action_RandomColor, &QAction::triggered, this, &QtApplication::RandomColor);
	QObject::connect(ui.action_PointCloudColorChange, &QAction::triggered, this, &QtApplication::PointColorChange);
	QObject::connect(ui.action_BackgroundColorChange, &QAction::triggered, this, &QtApplication::BackgroundColorChange);
	QObject::connect(ui.action_MainView, &QAction::triggered, this, &QtApplication::MainView);
	QObject::connect(ui.action_LeftView, &QAction::triggered, this, &QtApplication::LeftView);
	QObject::connect(ui.action_TopView, &QAction::triggered, this, &QtApplication::TopView);
	// ��ͼ(connect)
	QObject::connect(ui.action_DataManager, &QAction::triggered, this, &QtApplication::HideDataDock);
	QObject::connect(ui.action_PropertiesManager, &QAction::triggered, this, &QtApplication::HidePropertiesDock);
	QObject::connect(ui.action_ConsoleManager, &QAction::triggered, this, &QtApplication::HideConsoleDock);
	QObject::connect(ui.action_RGBManager, &QAction::triggered, this, &QtApplication::HideRGBDock);
	// ����(connect)
	QObject::connect(ui.action_Surface, &QAction::triggered, this, &QtApplication::Surface);
	QObject::connect(ui.action_Wireframe, &QAction::triggered, this, &QtApplication::Wireframe);
	// ���Ƽ�(connect)
	QObject::connect(ui.action_Simplify, &QAction::triggered, this, &QtApplication::Simplify);
	// ������ȡ(connect)
	QObject::connect(ui.action_Boundary, &QAction::triggered, this, &QtApplication::Boundary);
	// ����(connect)
	QObject::connect(ui.action_Help, &QAction::triggered, this, &QtApplication::Help);
	QObject::connect(ui.action_About,&QAction::triggered, this, &QtApplication::About);

	// ��������connect��
	QObject::connect(ui.actionOpen, &QAction::triggered, this, &QtApplication::Open);
	QObject::connect(ui.actionAdd, &QAction::triggered,this,&QtApplication::Add);
	QObject::connect(ui.actionClear, &QAction::triggered, this, &QtApplication::Clear);
	QObject::connect(ui.actionSave, &QAction::triggered, this, &QtApplication::Save);
	//QObject::connect(ui.actionSaveAs, &QAction::triggered, this, &QtApplication::SaveAsBinary);
	QObject::connect(ui.actionRandomColor,&QAction::triggered,this,&QtApplication::RandomColor);
	QObject::connect(ui.actionPointCloudColorChange, &QAction::triggered, this, &QtApplication::PointColorChange);
	QObject::connect(ui.actionBackgroundColorChange, &QAction::triggered, this, &QtApplication::BackgroundColorChange);
	QObject::connect(ui.actionSurface,&QAction::triggered,this,&QtApplication::Surface);
	QObject::connect(ui.actionWireframe, &QAction::triggered, this, &QtApplication::Wireframe);
	QObject::connect(ui.actionHelp, &QAction::triggered, this, &QtApplication::Help);
	QObject::connect(ui.actionAbout, &QAction::triggered, this, &QtApplication::About);
	
	//


	/***** ��ɫ�������źźͲۺ������� *****/
	// RGB��ɫ����(connect)
	connect(ui.Red_Slider, SIGNAL(valueChanged(int)), this, SLOT(Red_SliderChanged(int)));
	connect(ui.Green_Slider, SIGNAL(valueChanged(int)), this, SLOT(Green_SliderChanged(int)));
	connect(ui.Blue_Slider, SIGNAL(valueChanged(int)), this, SLOT(Blue_SliderChanged(int)));
	connect(ui.Red_Slider, SIGNAL(sliderReleased()), this, SLOT(RGBsliderReleased()));
	connect(ui.Green_Slider, SIGNAL(sliderReleased()), this, SLOT(RGBsliderReleased()));
	connect(ui.Blue_Slider, SIGNAL(sliderReleased()), this, SLOT(RGBsliderReleased()));
	// ���ƴ�С����(connect)
	connect(ui.PointSize_Slider, SIGNAL(valueChanged(int)), this, SLOT(PointSize_SliderChanged(int)));
	connect(ui.PointSize_Slider, SIGNAL(sliderReleased()), this, SLOT(PointSize_SliderReleased()));
	//����ϵ�ͱ�����ɫ����(connect)
	connect(ui.Cordinate, SIGNAL(stateChanged(int)), this, SLOT(CordinateChecked(int)));
	connect(ui.Background, SIGNAL(stateChanged(int)), this, SLOT(BackgroundChecked(int)));

	/***** �����ļ��������źźͲۺ������� *****/
	// ������������ļ�(connect)
	connect(ui.tree_PointCloud, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(dataSelected(QTreeWidgetItem*, int)));
	// �Ҽ����������ļ����Ҽ���Ӧ�˵� (connect)
	connect(ui.tree_PointCloud, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popMenu_DataTree(const QPoint&)));
	
	/***** ������¼�������źźͲۺ������� *****/
	connect(ui.table_Console, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(popMenu_Console(const QPoint&)));
}

QtApplication::~QtApplication()
{

}



/*****  һЩʵ�÷���ʵ�� *****/

//��ʼ��
void QtApplication::initial()
{
	//�����ʼ��
	setWindowIcon(QIcon(tr(":/Icon/Resources/Icon/Icon.png")));
	setWindowTitle(tr("My Graduate Design"));

	//���Ƴ�ʼ��
	mycloud.cloud.reset(new PointCloudT);
	mycloud.cloud->resize(1);//RGBA���Ƴ�ʼ������height=1�������
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	viewer->addPointCloud(mycloud.cloud, "cloud");


	cloud_xyz.reset(new pcl::PointCloud<pcl::PointXYZ>);
	cloud_xyz->resize(1);
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	viewer->addPointCloud(cloud_xyz, "cloud");

	ui.qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui.qvtkWidget->GetInteractor(), ui.qvtkWidget->GetRenderWindow());
	ui.qvtkWidget->update();
	
	//���ڿؼ���ʼ��
	ui.table_Properties->setSelectionMode(QAbstractItemView::NoSelection); // ��ֹ������Թ������� item
	ui.table_Console->setSelectionMode(QAbstractItemView::NoSelection);  // ��ֹ���������ڵ� item
	ui.tree_PointCloud->setSelectionMode(QAbstractItemView::ExtendedSelection); // ���� dataTree ���ж�ѡ

	setPropertyTable(0, 0, 0, " ");
	//setPropertyTable();//��ʼ�����Թ�����
	setConsoleTable();//��ʼ��������¼����

	// �������
	consoleLog(QString::fromLocal8Bit("��ʼ"), "My Graduate Design", "Welcome to My Graduate Design", "DMY");


	// ���ñ�����ɫΪ dark
	viewer->setBackgroundColor(30 / 255.0, 30 / 255.0, 30 / 255.0);
}

//��ʾ���ƣ�����������Ƕ�
void QtApplication::showPointcloud()
{
	for (int i = 0; i != mycloud_vec.size(); i++)
	{
		viewer->updatePointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
	}
	//viewer->resetCamera();
	ui.qvtkWidget->update();
}

//��ӵ��Ƶ�viewer,����ʾ����
void QtApplication::showPointcloudAdd()
{
	for (int i = 0; i != mycloud_vec.size(); i++)
	{
		viewer->addPointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
		viewer->updatePointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
	}
	viewer->resetCamera();//��������Ƕ�
	ui.qvtkWidget->update();
}

//�򿪵����ļ�ʱ�����õ�����ɫ
void QtApplication::setCloudColor(unsigned int r, unsigned int g, unsigned int b)
{
	// Set the new color
	for (size_t i = 0; i < mycloud.cloud->size(); i++)
	{
		mycloud.cloud->points[i].r = r;
		mycloud.cloud->points[i].g = g;
		mycloud.cloud->points[i].b = b;
		mycloud.cloud->points[i].a = 255;
	}
}

//�򿪵����ļ�ʱ�����õ���͸����
void QtApplication::setA(unsigned int a)
{
	for (size_t i = 0; i < mycloud.cloud->size(); i++)
	{
		mycloud.cloud->points[i].a = a;
	}
}

//�������Թ�����
void QtApplication::setPropertyTable(int clouds, int points, int totalpoints, QString RGB)
{

	QStringList header;
	header << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("ֵ");
	ui.table_Properties->setHorizontalHeaderLabels(header);
	ui.table_Properties->setItem(0, 0, new QTableWidgetItem(QString::fromLocal8Bit("������")));
	ui.table_Properties->setItem(1, 0, new QTableWidgetItem(QString::fromLocal8Bit("����")));
	ui.table_Properties->setItem(2, 0, new QTableWidgetItem(QString::fromLocal8Bit("�ܵ���")));
	ui.table_Properties->setItem(3, 0, new QTableWidgetItem(QString::fromLocal8Bit("��ɫ")));


	ui.table_Properties->setItem(0, 1, new QTableWidgetItem(QString::number(clouds)));
	ui.table_Properties->setItem(1, 1, new QTableWidgetItem(QString::number(points)));
	ui.table_Properties->setItem(2, 1, new QTableWidgetItem(QString::number(totalpoints)));
	ui.table_Properties->setItem(3, 1, new QTableWidgetItem(RGB));

}

//��ʼ�����ò�����¼������
void QtApplication::setConsoleTable(){
	// �����������
	QStringList header2;
	header2 << QString::fromLocal8Bit("ʱ��") << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("��������") << QString::fromLocal8Bit("��ϸ") << QString::fromLocal8Bit("��ע");
	ui.table_Console->setHorizontalHeaderLabels(header2);
	ui.table_Console->setColumnWidth(0, 140);
	ui.table_Console->setColumnWidth(1, 180);
	ui.table_Console->setColumnWidth(2, 180);
	ui.table_Console->setColumnWidth(3, 280);

	ui.table_Console->verticalHeader()->setDefaultSectionSize(22); //�����о�

	ui.table_Console->setContextMenuPolicy(Qt::CustomContextMenu);

}

//����RGBDock����
void QtApplication::setRGBDock()
{
	ui.Red_lcdNumber->setProperty("value", QVariant(255));
	ui.Red_Slider->setSliderPosition(255);
	ui.Green_lcdNumber->setProperty("value", QVariant(255));
	ui.Green_Slider->setSliderPosition(255);
	ui.Blue_lcdNumber->setProperty("value", QVariant(255));
	ui.Blue_Slider->setSliderPosition(255);
	ui.PointSize_lcdNumber->setProperty("value", QVariant(1));
	ui.PointSize_Slider->setSliderPosition(1);
	ui.Cordinate->setCheckState(Qt::CheckState::Unchecked);
	viewer->removeCoordinateSystem();
	ui.Background->setCheckState(Qt::CheckState::Unchecked);
	viewer->setBackgroundColor(30 / 255.0, 30 / 255.0, 30 / 255.0);
}

//��¼������ʾ�ڿ���̨
void QtApplication::consoleLog(QString operation, QString subname, QString filename, QString note)
{
	if (enable_console == false){
		return;
	}
	int rows = ui.table_Console->rowCount();
	ui.table_Console->setRowCount(++rows);
	QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
	QString time_str = time.toString("MM-dd hh:mm:ss"); //������ʾ��ʽ
	ui.table_Console->setItem(rows - 1, 0, new QTableWidgetItem(time_str));
	ui.table_Console->setItem(rows - 1, 1, new QTableWidgetItem(operation));
	ui.table_Console->setItem(rows - 1, 2, new QTableWidgetItem(subname));
	ui.table_Console->setItem(rows - 1, 3, new QTableWidgetItem(filename));
	ui.table_Console->setItem(rows - 1, 4, new QTableWidgetItem(note));

	ui.table_Console->scrollToBottom(); // �����Զ�������ײ�
}



/*****    �ļ��˵�����ʵ��   *****/

//�򿪵����ļ���֧��������.pcd ��.ply ��.obj�����ָ�ʽ�����ļ�
void QtApplication::Open()
{
	QStringList filenames = QFileDialog::getOpenFileNames(
		this, 
		QString::fromLocal8Bit("�򿪵����ļ�"), //����
		QString::fromLocal8Bit(mycloud.dirname.c_str()), //·��
		QString::fromLocal8Bit("�����ļ�(*.pcd *.ply *.obj);;ȫ���ļ�(*.*)"));//��������ɸѡ
	//Return if filenames is empty
	if (filenames.isEmpty())  return;
	mycloud_vec.clear();
	total_points = 0;
	ui.tree_PointCloud->clear();
	viewer->removeAllPointClouds();

	// �����򿪵����ļ�
	for (int i = 0; i != filenames.size(); i++)
	{
		timer.restart();//��ʼ��ʱ
		mycloud.cloud.reset(new PointCloudT); // Reset cloud
		QString filename = filenames[i];
		std::string file_name = filename.toStdString();
		std::string subname = getFileName(file_name);  //��ȡȫ·���е��ļ���������׺��
		
		int status = -1;
		if (filename.endsWith(".pcd", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPCDFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else if (filename.endsWith(".ply", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPLYFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else if (filename.endsWith(".obj", Qt::CaseInsensitive))
		{
			status = pcl::io::loadOBJFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else
		{
			//��ʾ���޷���ȡ����.ply .pcd .obj������ļ�
			QMessageBox::information(this, 
				QString::fromLocal8Bit("�ļ���ʽ����"),
				QString::fromLocal8Bit("�޷��򿪳�.ply .pcd .obj���ʽ���ļ���"));
			return;
		}
		//��ʾ����׺û���⣬���ļ������޷���ȡ
		if (status != 0)
		{
			QMessageBox::critical(this, 
				QString::fromLocal8Bit("���ļ�����"),
				QString::fromLocal8Bit("�޷��򿪸��ļ���"));
			return;
		}
		setA(255);  //���õ���Ϊ��͸��		
		// ���һ�����Ƶ���Ϣ
		mycloud.filename = file_name;
		mycloud.subname = subname;
		mycloud.dirname = file_name.substr(0, file_name.size() - subname.size());
		mycloud_vec.push_back(mycloud);  //�����Ƶ����������
		
		// time off
		//time_action = timer.elapsed();
		// �������
		consoleLog(QString::fromLocal8Bit("��"), 
			QString::fromLocal8Bit(mycloud.subname.c_str()), 
			QString::fromLocal8Bit(mycloud.filename.c_str()), 
			QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed()/1000.0)+ QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud.cloud->points.size()));


		//������Դ������
		QTreeWidgetItem *cloudName = new QTreeWidgetItem(QStringList()<< QString::fromLocal8Bit(subname.c_str()));
		cloudName->setIcon(0, QIcon(":/Icon/Resources/Icon/Icon.png"));//������Դ��ͼ��
		ui.tree_PointCloud->addTopLevelItem(cloudName);


		//setWindowTitle(filename + " - CloudViewer"); //���±���

		total_points += mycloud.cloud->points.size();
	}
	//������Դ�����Ϣ����ǰ���Ƹ����������ܵ���������Ĭ����ɫ��255 255 255����

	if (mycloud_vec.size() == 1)
	{
		setPropertyTable(mycloud_vec.size(), mycloud_vec[0].cloud->points.size(), total_points, "255 255 255");
	}
	else
	{
		setPropertyTable(mycloud_vec.size(), 0, total_points, "255 255 255");
	}

	ui.statusBar->showMessage(QString::fromLocal8Bit("��"));
	showPointcloudAdd();  //������ͼ����

}

//��ӵ����ļ���
void QtApplication::Add()
{
	QStringList filenames = QFileDialog::getOpenFileNames(this, 
		QString::fromLocal8Bit("��ӵ����ļ�"), //����
		QString::fromLocal8Bit(mycloud.dirname.c_str()), //·��
		QString::fromLocal8Bit("�����ļ�(*.pcd *.ply *.obj);;ȫ���ļ�(*.*)"));//��������ɸѡ);
	if (filenames.isEmpty())  return;//
	// �����򿪵����ļ�
	for (int i = 0; i != filenames.size(); i++)
	{
		timer.restart();//��ʼ��ʱ
		mycloud.cloud.reset(new PointCloudT); // Reset cloud
		QString filename = filenames[i];
		std::string file_name = filename.toStdString();
		std::string subname = getFileName(file_name);  //��ȡȫ·���е��ļ���������׺��

		int status = -1;
		if (filename.endsWith(".pcd", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPCDFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else if (filename.endsWith(".ply", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPLYFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else if (filename.endsWith(".obj", Qt::CaseInsensitive))
		{
			status = pcl::io::loadOBJFile(file_name, *(mycloud.cloud));
			if (mycloud.cloud->points[0].r == 0 && mycloud.cloud->points[0].g == 0 && mycloud.cloud->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else
		{
			//��ʾ���޷���ȡ����.ply .pcd .obj������ļ�
			QMessageBox::information(this,
				QString::fromLocal8Bit("�ļ���ʽ����"),
				QString::fromLocal8Bit("�޷���ӳ�.ply .pcd .obj���ʽ���ļ���"));
			return;
		}
		//��ʾ����׺û���⣬���ļ������޷���ȡ
		if (status != 0)
		{
			QMessageBox::critical(this,
				QString::fromLocal8Bit("����ļ�����"),
				QString::fromLocal8Bit("�޷���Ӹ��ļ���"));
			return;
		}
		setA(255);  //���õ���Ϊ��͸��		
		// ���һ�����Ƶ���Ϣ
		mycloud.filename = file_name;
		mycloud.subname = subname;
		mycloud.dirname = file_name.substr(0, file_name.size() - subname.size());
		mycloud_vec.push_back(mycloud);  //�����Ƶ����������

		// �������
		consoleLog(QString::fromLocal8Bit("���"),
			QString::fromLocal8Bit(mycloud.subname.c_str()),
			QString::fromLocal8Bit(mycloud.filename.c_str()),
			QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed() / 1000.0) + QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud.cloud->points.size()));


		//������Դ������
		QTreeWidgetItem *cloudName = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit(subname.c_str()));
		cloudName->setIcon(0, QIcon(":/Icon/Resources/Icon/Icon.png"));//������Դ��ͼ��
		ui.tree_PointCloud->addTopLevelItem(cloudName);


		//setWindowTitle(filename + " - CloudViewer"); //���±���

		total_points += mycloud.cloud->points.size();
	}
	setPropertyTable(mycloud_vec.size(), 0, total_points, "255 255 255");
	ui.statusBar->showMessage(QString::fromLocal8Bit("���"));
	showPointcloudAdd();  //������ͼ����
}

//��������ļ���֧��.pcd ��.ply���ָ�ʽ�����ļ�
void QtApplication::Save()
{
	QString save_filename = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit("��������ļ�"),
		QString::fromLocal8Bit(mycloud.dirname.c_str()), 
		QString::fromLocal8Bit("�����ļ�(*.pcd *.ply);;ȫ���ļ�(*.*)"));
	

	//�ļ���Ϊ��ֱ�ӷ���
	if (save_filename.isEmpty())
		return;
	else
	{	
		std::string file_name = save_filename.toStdString();
		std::string subname = getFileName(file_name);
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		int selected_item_count = ui.tree_PointCloud->selectedItems().size();
		// ���δѡ���κε��ƣ������ͼ�����е����е����޸Ĵ�С
		if (mycloud_vec.size()==1||selected_item_count == 1)
		{
			//�ж��ļ���������
			int status = -1;
			if (save_filename.endsWith(".pcd", Qt::CaseInsensitive))//CaseInsensitive(�����ִ�Сд)
			{
				status = pcl::io::savePCDFile(file_name, *(mycloud.cloud));
			}
			else if (save_filename.endsWith(".ply", Qt::CaseInsensitive))
			{
				status = pcl::io::savePLYFile(file_name, *(mycloud.cloud));
			}
			else //��ʾ���޷�����Ϊ����.ply .pcd������ļ�
			{
				QMessageBox::information(this,
					QString::fromLocal8Bit("�ļ���ʽ����"),
					QString::fromLocal8Bit("�޷�����Ϊ��.ply .pcd���ʽ���ļ�"));
				return;
			}
			consoleLog(QString::fromLocal8Bit("����"), QString::fromStdString(subname), QString::fromLocal8Bit("��������ļ�"), "");

		}
		else
		{
			Savemulti(save_filename);
		}	
		QMessageBox::information(this,
			QString::fromLocal8Bit("��������ļ�"),
			QString::fromLocal8Bit(("����" + subname + "�ɹ���").c_str()));
	}
	ui.statusBar->showMessage(QString::fromLocal8Bit("����"));
}

//����������ݱ���Ϊһ���ļ�
void QtApplication::Savemulti(QString savename)
{
	std::string subname = getFileName(savename.toStdString());
	PointCloudT::Ptr multi_cloud;
	multi_cloud.reset(new PointCloudT);
	multi_cloud->height = 1;//�������
	int sum = 0;
	for (auto c : mycloud_vec)
	{
		sum += c.cloud->points.size();
	}
	multi_cloud->width = sum;
	multi_cloud->resize(multi_cloud->height * multi_cloud->width);
	int k = 0;
	for (int i = 0; i != mycloud_vec.size(); i++)//������
	{
		//ע��cloudvec[i]->points.size()��cloudvec[i]->size()������
		for (int j = 0; j != mycloud_vec[i].cloud->points.size(); j++)    //�����ܵ���     
		{
			multi_cloud->points[k].x = mycloud_vec[i].cloud->points[j].x;
			multi_cloud->points[k].y = mycloud_vec[i].cloud->points[j].y;
			multi_cloud->points[k].z = mycloud_vec[i].cloud->points[j].z;
			multi_cloud->points[k].r = mycloud_vec[i].cloud->points[j].r;
			multi_cloud->points[k].g = mycloud_vec[i].cloud->points[j].g;
			multi_cloud->points[k].b = mycloud_vec[i].cloud->points[j].b;
			k++;
		}
	}
	//����multi_cloud
	int status = -1;
	if (savename.endsWith(".pcd", Qt::CaseInsensitive))
	{
		status = pcl::io::savePCDFile(savename.toStdString(), *multi_cloud);
	}
	else if (savename.endsWith(".ply", Qt::CaseInsensitive))
	{
		status = pcl::io::savePLYFile(savename.toStdString(), *multi_cloud);
	}
	else //��ʾ���޷�����Ϊ����.ply .pcd������ļ�
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("�ļ���ʽ����"),
			QString::fromLocal8Bit("�޷�����Ϊ��.ply .pcd���ʽ���ļ�"));
		return;
	}
	// �������
	consoleLog(QString::fromLocal8Bit("����"), QString::fromLocal8Bit(subname.c_str()), savename, QString::fromLocal8Bit("����������ݱ���Ϊһ���ļ�"));

	//�������� multi_cloud ����Ϊ��ǰ mycloud,�Ա㱣��֮��ֱ�ӽ��в���
	mycloud.cloud = multi_cloud;
	mycloud.filename = savename.toStdString();
	mycloud.subname = subname;
}

//��յ���
void QtApplication::Clear()
{
	if (mycloud_vec.size())
	{
	    mycloud_vec.clear();//��յ�������
		viewer->removeAllPointClouds();//��Viewer���Ƴ����е���;
		viewer->removeAllShapes();//��ո����ף��Ƴ�����
		ui.tree_PointCloud->clear();  //��dataTree���

		total_points = 0;
		setPropertyTable(0,0,0," ");//�������Թ�����
		setRGBDock();//����RGBDock����
		// �������	
		consoleLog(QString::fromLocal8Bit("���"), QString::fromLocal8Bit("���е���"), "", "DMY");
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��������е���!"));		
		showPointcloud();  //������ʾ
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ǰδ�����κε����ļ���"));
		return;
	}
}

//�˳�����
void QtApplication::Exit()
{
	this->close();
}




/*****    ��ʾ�˵�����ʵ��   *****/
//��ʾ�˵������������ɫ
void QtApplication::RandomColor()
{
	if (mycloud_vec.size())////��ǰ�е����ļ������������ɫ
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		int selected_item_count = ui.tree_PointCloud->selectedItems().size();

		// ���δѡ���κε��ƣ������ͼ�����е����е��ƽ�����ɫ
		if (selected_item_count == 0)
		{
			for (int i = 0; i != mycloud_vec.size(); i++){
				for (int j = 0; j != mycloud_vec[i].cloud->points.size(); j++){
					mycloud_vec[i].cloud->points[j].r = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
					mycloud_vec[i].cloud->points[j].g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
					mycloud_vec[i].cloud->points[j].b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
				}
			}

			// �������
			consoleLog(QString::fromLocal8Bit("���õ�����ɫ"),
				QString::fromLocal8Bit("���е���"),
				QString::fromLocal8Bit("������õ�����ɫ"), "DMY");
		}
		else
		{
			for (int i = 0; i != selected_item_count; i++)
			{
				int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
				for (int j = 0; j != mycloud_vec[cloud_id].cloud->size(); j++){
					mycloud_vec[cloud_id].cloud->points[j].r = red;
					mycloud_vec[cloud_id].cloud->points[j].g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
					mycloud_vec[cloud_id].cloud->points[j].b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
				}
			}
			// �������
			consoleLog(QString::fromLocal8Bit("���õ�����ɫ"),
				QString::fromLocal8Bit("��ѡ�е���"),
				QString::fromLocal8Bit("������õ�����ɫ"), "DMY");

		}
		showPointcloud();
		ui.table_Properties->setItem(3, 1, new QTableWidgetItem(QString::fromLocal8Bit("�����ɫ")));


	}
	else//��ǰδ���ص����ļ�
	{
		//��ʾ������ӵ����ļ�
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ǰδ�����κε����ļ���"));
		return;
	}

}

//��ʾ�˵�ͨ����ɫ�Ի����޸ĵ�����ɫ
void QtApplication::PointColorChange()
{
	if (mycloud_vec.size())//��ǰ�е����ļ���������ɫ
	{
		QColor color_select = QColorDialog::getColor(Qt::white, this, QString::fromLocal8Bit("ѡ�������ɫ"));
		if (color_select.isValid())//�ж���ѡ����ɫ�Ƿ���Ч
		{
			QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
			int selected_item_count = ui.tree_PointCloud->selectedItems().size();
			// ���δѡ���κε��ƣ������ͼ�����е����е��ƽ�����ɫ
			if (selected_item_count == 0)
			{
				for (int i = 0; i != mycloud_vec.size(); i++)
				{
					for (int j = 0; j != mycloud_vec[i].cloud->points.size(); j++)
					{
						mycloud_vec[i].cloud->points[j].r = color_select.red();
						mycloud_vec[i].cloud->points[j].g = color_select.green();
						mycloud_vec[i].cloud->points[j].b = color_select.blue();
					}
				}
				// �������
				consoleLog(QString::fromLocal8Bit("���õ�����ɫ"), QString::fromLocal8Bit("���е���"), QString::number(color_select.red()) + " " + QString::number(color_select.green()) + " " + QString::number(color_select.blue()), "");
			}
			else
			{
				for (int i = 0; i != selected_item_count; i++)
				{
					int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
					for (int j = 0; j != mycloud_vec[cloud_id].cloud->size(); j++)
					{
						mycloud_vec[cloud_id].cloud->points[j].r = color_select.red();
						mycloud_vec[cloud_id].cloud->points[j].g = color_select.green();
						mycloud_vec[cloud_id].cloud->points[j].b = color_select.blue();
					}
				}
				// �������
				consoleLog(QString::fromLocal8Bit("���õ�����ɫ"), QString::fromLocal8Bit("��ѡ�е���"), QString::number(color_select.red()) + " " + QString::number(color_select.green()) + " " + QString::number(color_select.blue()), "");
			}
			showPointcloud();
			//���Դ�����ɫ�ı�
			ui.table_Properties->setItem(3, 1, new QTableWidgetItem(QString::number(color_select.red()) + " " + QString::number(color_select.green()) + " " + QString::number(color_select.blue())));
			//��ɫ�ĸı�ͬ����RGB��ɫ������
			ui.Red_Slider->setValue(color_select.red());
			ui.Green_Slider->setValue(color_select.green());
			ui.Blue_Slider->setValue(color_select.blue());
		}
	}
	else  //��ǰδ���ص����ļ�
	{
		ui.Red_Slider->setValue(255);
		ui.Green_Slider->setValue(255);
		ui.Blue_Slider->setValue(255);
		//��ʾ������ӵ����ļ�
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ǰδ�����κε����ļ���"));
		return;
	}
}

//��ʾ�˵�ͨ����ɫ�Ի����޸ı�����ɫ
void QtApplication::BackgroundColorChange()
{
	QColor color_select = QColorDialog::getColor(Qt::white, this, QString::fromLocal8Bit("ѡ�񱳾���ɫ"));
	if (color_select.isValid())//�ж���ѡ����ɫ�Ƿ���Ч
	{
		viewer->setBackgroundColor(color_select.red() / 255.0, color_select.green() / 255.0, color_select.blue() / 255.0);
		// �������
		consoleLog(QString::fromLocal8Bit("�ı䱳����ɫ"), QString::fromLocal8Bit("������ɫ"), QString::number(color_select.red()) + " " + QString::number(color_select.green()) + " " + QString::number(color_select.blue()), "");
		ui.qvtkWidget->update();
		//showPointcloud();
	}
}

//���ӽǣ����ӽǣ����ӽ�
void QtApplication::MainView()
{
	viewer->setCameraPosition(0, -1, 0, 0.5, 0.5, 0.5, 0, 0, 1);
	ui.qvtkWidget->update();
}

void QtApplication::LeftView()
{
	viewer->setCameraPosition(-1, 0, 0, 0, 0, 0, 0, 0, 1);
	ui.qvtkWidget->update();
}

void QtApplication::TopView()
{
	viewer->setCameraPosition(0, 0, 1, 0, 0, 0, 0, 1, 0);
	ui.qvtkWidget->update();
}



/*****    ��ͼ�˵�����ʵ��   *****/
//����ͣ�����ڵ���ʾ������
void QtApplication::HideDataDock()
{
	if (ui.action_DataManager->isChecked())
	{
		ui.dock_datatree->setVisible(true);
	}
	else
	{
		ui.dock_datatree->setVisible(false);
	}
}

void QtApplication::HidePropertiesDock()
{
	if (ui.action_PropertiesManager->isChecked())
	{
		ui.dock_properties->setVisible(true);
	}
	else
	{
		ui.dock_properties->setVisible(false);
	}
}

void QtApplication::HideConsoleDock()
{
	if (ui.action_ConsoleManager->isChecked())
	{
		ui.dock_console->setVisible(true);
	}
	else
	{
		ui.dock_console->setVisible(false);
	}
}

void QtApplication::HideRGBDock()
{
	if (ui.action_RGBManager->isChecked())
	{
		ui.dock_rgbcolor->setVisible(true);
	}
	else
	{
		ui.dock_rgbcolor->setVisible(false);
	}
}


/*****    ����˵�����ʵ��   *****/

//����������Ƭ
void QtApplication::Surface()
{

	QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
	int selected_item_count = ui.tree_PointCloud->selectedItems().size();
	if (selected_item_count == 0)
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ѡ��һ�������ļ�"));
		return;
	}
	else
	{
		for (int i = 0; i != selected_item_count; i++)
		{
			int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
			/* ����
			����÷���ֻ�ܴ���PointXYZ�ĵ��ƣ���PointXZYRGBA�ĵ��Ʊ���ᱨ��
			����boost::this_thread::sleep����Ҳ��������
			����Ƚ��е�������ת��
			*/
			pcl::PointXYZ point;
			cloud_xyz.reset(new pcl::PointCloud<pcl::PointXYZ>);
			timer.restart();//��ʼ��ʱ
			//��mycloud.cloud������XYZRGBA�ĵ��ƣ�ת����cloud_xyz��ֻ����XYZ�ĵ��ƣ�
			//int number = mycloud_vec[cloud_id].cloud->size();
			for (size_t i = 0; i != mycloud_vec[cloud_id].cloud->size(); i++)
			{
				point.x = mycloud_vec[cloud_id].cloud->points[i].x;
				point.y = mycloud_vec[cloud_id].cloud->points[i].y;
				point.z = mycloud_vec[cloud_id].cloud->points[i].z;
				cloud_xyz->push_back(point);//���������ӽ�cloud_xyz������
			}
			if (!cloud_xyz)
			{
				return;
			}
			/****** �������ģ�� ******/
			//�������߹��ƶ��� n
			////Normal�ṹ���ʾ�������������������ϵķ��߷����Լ���Ӧ���ʵĲ���ֵ
			pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
			//������������ָ�� normals
			pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
			//����kdtree���ڷ������ʱ�ٽ�����
			pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
			kdtree->setInputCloud(cloud_xyz);//Ϊkdtree�������
			n.setInputCloud(cloud_xyz);//Ϊ������ƶ����������
			n.setSearchMethod(kdtree);//���÷������ʱ����ȡ��������ʽkdtree
			n.setKSearch(20);//K������������
			n.compute(*normals);

			//QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("���߹��������"));

			/****** ���������뷨������ƴ�� ******/
			//����ͬʱ������ͷ��ߵ����ݽṹ��ָ��
			//PointNormal�Ǵ洢XYZ���ݵ�point�ṹ�壬���Ұ����˲�����ķ��ߺ�����
			pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
			//���ѻ�õĵ������ݺͷ�����������//������������cloud�������йأ��ĳ�PointXYZ�ĵ��ƾ�û�б�����
			pcl::concatenateFields(*cloud_xyz, *normals, *cloud_with_normals);
			//������һ��kdtree�����ؽ�
			pcl::search::KdTree<pcl::PointNormal>::Ptr kdtree2(new pcl::search::KdTree<pcl::PointNormal>);
			//Ϊkdtree2�������,���õ�����������Ϊ��ͷ���
			kdtree2->setInputCloud(cloud_with_normals);

			/****** �����ؽ�ģ�� ******/
			//����̰��������ͶӰ�ؽ�����
			pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
			//���������������������洢�ؽ����
			pcl::PolygonMesh mesh;
			//���ò���
			gp3.setSearchRadius(25);//�������ӵ�֮�������룬����ȷ��k���ڵ���뾶
			gp3.setMu(2.5);//��������ھ���ĳ��ӣ��ѵõ�ÿ��������������뾶
			gp3.setMaximumNearestNeighbors(100);//�������������ڽ�����������
			gp3.setMaximumSurfaceAngle(M_PI / 2);//45�ȣ����ƽ���
			gp3.setMinimumAngle(M_PI / 18);//10�ȣ�ÿ�����ǵ���С�Ƕ�
			gp3.setMaximumAngle(2 * M_PI / 3);//120��,ÿ�����ǵ����Ƕ�
			gp3.setNormalConsistency(false);//��������һ�£���Ϊtrue
			//���õ������ݺ�������ʽ
			gp3.setInputCloud(cloud_with_normals);
			gp3.setSearchMethod(kdtree2);
			gp3.reconstruct(mesh);
			//QMessageBox::information(this, "informaiton", "Reconstruction finished");
			//���ؽ�������浽Ӳ���ļ��У��ؽ������VTK��ʽ�洢
			pcl::io::saveVTKFile("mymesh.vtk", mesh);

			/****** ͼ����ʾģ�� ******/
			//QMessageBox::information(this, "information", "Start to show");
			viewer->addPolygonMesh(mesh, "my");//����Ҫ��ʾ���������
			//��������ģ����ʾģʽ
			//viewer->setRepresentationToPointsForAllActors();//����ģ���Ե���ʽ��ʾ
			viewer->setRepresentationToSurfaceForAllActors();//����ģ������Ƭ��ʽ��ʾ
			//viewer->setRepresentationToWireframeForAllActors();//����ģ�����߿�ͼģʽ��ʾ

			//�������
			consoleLog(QString::fromLocal8Bit("����������Ƭ"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].filename.c_str()),
				QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed() / 1000.0) + QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud_vec[cloud_id].cloud->size()));
			//viewer->removeAllShapes();
			while (!viewer->wasStopped())
			{
				viewer->spinOnce(100);
				//boost::this_thread::sleep(boost::posix_time::microseconds(100000));
			}
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ��Ϣ"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()) + QString::fromLocal8Bit("���Ǹ����������!")
				);
		}
	}
}

//���������߿�	
void QtApplication::Wireframe()
{

	QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
	int selected_item_count = ui.tree_PointCloud->selectedItems().size();
	if (selected_item_count == 0)
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ѡ��һ�������ļ�"));
		return;
	}
	else
	{
		for (int i = 0; i != selected_item_count; i++)
		{
			int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
			/* ����
			����÷���ֻ�ܴ���PointXYZ�ĵ��ƣ���PointXZYRGBA�ĵ��Ʊ���ᱨ��
			����boost::this_thread::sleep����Ҳ��������
			����Ƚ��е�������ת��
			*/
			pcl::PointXYZ point;
			cloud_xyz.reset(new pcl::PointCloud<pcl::PointXYZ>);
			timer.restart();//��ʼ��ʱ
			//��mycloud.cloud������XYZRGBA�ĵ��ƣ�ת����cloud_xyz��ֻ����XYZ�ĵ��ƣ�
			//int number = mycloud_vec[cloud_id].cloud->size();
			for (size_t i = 0; i != mycloud_vec[cloud_id].cloud->size(); i++)
			{
				point.x = mycloud_vec[cloud_id].cloud->points[i].x;
				point.y = mycloud_vec[cloud_id].cloud->points[i].y;
				point.z = mycloud_vec[cloud_id].cloud->points[i].z;
				cloud_xyz->push_back(point);//���������ӽ�cloud_xyz������
			}
			if (!cloud_xyz)
			{
				return;
			}
			/****** �������ģ�� ******/
			//�������߹��ƶ��� n
			////Normal�ṹ���ʾ�������������������ϵķ��߷����Լ���Ӧ���ʵĲ���ֵ
			pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
			//������������ָ�� normals
			pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
			//����kdtree���ڷ������ʱ�ٽ�����
			pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
			kdtree->setInputCloud(cloud_xyz);//Ϊkdtree�������
			n.setInputCloud(cloud_xyz);//Ϊ������ƶ����������
			n.setSearchMethod(kdtree);//���÷������ʱ����ȡ��������ʽkdtree
			n.setKSearch(20);//K������������
			n.compute(*normals);

			//QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("���߹��������"));

			/****** ���������뷨������ƴ�� ******/
			//����ͬʱ������ͷ��ߵ����ݽṹ��ָ��
			//PointNormal�Ǵ洢XYZ���ݵ�point�ṹ�壬���Ұ����˲�����ķ��ߺ�����
			pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
			//���ѻ�õĵ������ݺͷ�����������//������������cloud�������йأ��ĳ�PointXYZ�ĵ��ƾ�û�б�����
			pcl::concatenateFields(*cloud_xyz, *normals, *cloud_with_normals);
			//������һ��kdtree�����ؽ�
			pcl::search::KdTree<pcl::PointNormal>::Ptr kdtree2(new pcl::search::KdTree<pcl::PointNormal>);
			//Ϊkdtree2�������,���õ�����������Ϊ��ͷ���
			kdtree2->setInputCloud(cloud_with_normals);

			/****** �����ؽ�ģ�� ******/
			//����̰��������ͶӰ�ؽ�����
			pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
			//���������������������洢�ؽ����
			pcl::PolygonMesh mesh;
			//���ò���
			gp3.setSearchRadius(25);//�������ӵ�֮�������룬����ȷ��k���ڵ���뾶
			gp3.setMu(2.5);//��������ھ���ĳ��ӣ��ѵõ�ÿ��������������뾶
			gp3.setMaximumNearestNeighbors(100);//�������������ڽ�����������
			gp3.setMaximumSurfaceAngle(M_PI / 2);//45�ȣ����ƽ���
			gp3.setMinimumAngle(M_PI / 18);//10�ȣ�ÿ�����ǵ���С�Ƕ�
			gp3.setMaximumAngle(2 * M_PI / 3);//120��,ÿ�����ǵ����Ƕ�
			gp3.setNormalConsistency(false);//��������һ�£���Ϊtrue
			//���õ������ݺ�������ʽ
			gp3.setInputCloud(cloud_with_normals);
			gp3.setSearchMethod(kdtree2);
			gp3.reconstruct(mesh);
			//QMessageBox::information(this, "informaiton", "Reconstruction finished");
			//���ؽ�������浽Ӳ���ļ��У��ؽ������VTK��ʽ�洢
			pcl::io::saveVTKFile("mymesh.vtk", mesh);

			/****** ͼ����ʾģ�� ******/
			//QMessageBox::information(this, "information", "Start to show");
			viewer->addPolygonMesh(mesh, "my");//����Ҫ��ʾ���������
			//��������ģ����ʾģʽ
			//viewer->setRepresentationToPointsForAllActors();//����ģ���Ե���ʽ��ʾ
			//viewer->setRepresentationToSurfaceForAllActors();//����ģ������Ƭ��ʽ��ʾ
			viewer->setRepresentationToWireframeForAllActors();//����ģ�����߿�ͼģʽ��ʾ

			//�������
			consoleLog(QString::fromLocal8Bit("���������߿�"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].filename.c_str()),
				QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed() / 1000.0) + QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud_vec[cloud_id].cloud->size()));
			viewer->removeAllShapes();
			while (!viewer->wasStopped())
			{
				viewer->spinOnce(100);
				//boost::this_thread::sleep(boost::posix_time::microseconds(100000));
			}
			QMessageBox::information(this, 
				QString::fromLocal8Bit("��ʾ��Ϣ"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()) + QString::fromLocal8Bit("���Ǹ����������!")
				);
		}
	}
}




/*****    ���Ƽ򻯲˵�����ʵ��   *****/
void QtApplication::Simplify()
{
	QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
	int selected_item_count = ui.tree_PointCloud->selectedItems().size();
	if (selected_item_count == 0)
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ѡ��һ�������ļ�"));
		return;
	}
	else
	{

		for (int i = 0; i != selected_item_count; i++)
		{
			timer.restart();//��ʼ��ʱ
			int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
			/* ����
			����÷���ֻ�ܴ���PointXYZ�ĵ��ƣ���PointXZYRGBA�ĵ��Ʊ���ᱨ��
			����boost::this_thread::sleep����Ҳ��������
			����Ƚ��е�������ת��
			*/
			pcl::PointXYZ point;
			cloud_xyz.reset(new pcl::PointCloud<pcl::PointXYZ>);
			timer.restart();//��ʼ��ʱ
			//��mycloud.cloud������XYZRGBA�ĵ��ƣ�ת����cloud_xyz��ֻ����XYZ�ĵ��ƣ�
			//int number = mycloud_vec[cloud_id].cloud->size();
			for (size_t i = 0; i != mycloud_vec[cloud_id].cloud->size(); i++)
			{
				point.x = mycloud_vec[cloud_id].cloud->points[i].x;
				point.y = mycloud_vec[cloud_id].cloud->points[i].y;
				point.z = mycloud_vec[cloud_id].cloud->points[i].z;
				cloud_xyz->push_back(point);//���������ӽ�cloud_xyz������
			}
			if (!cloud_xyz)
			{
				return;
			}

			srand(time(0));
			// Read in the cloud data
			pcl::PCDReader reader;
			//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_f(new pcl::PointCloud<pcl::PointXYZ>);
			//reader.read("table_scene_lms400.pcd", *cloud_filtered);
			//cloud_filtered
			//std::cout << "PointCloud has: " << cloud_filtered->points.size() << " data points." << std::endl; //*


			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ��Ϣ"),
				QString::fromLocal8Bit("һ���У�") + QString::number(cloud_xyz->points.size(), 10) + QString::fromLocal8Bit("����"));
			//���
			ofstream fout("E:\\Date\\PointCloud\\temp\\plane.txt");
			int point_num = 0;
			// Create the segmentation object for the planar model and set all the parameters
			pcl::SACSegmentation<pcl::PointXYZ> seg;
			pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
			pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ>());
			pcl::PCDWriter writer;
			seg.setOptimizeCoefficients(true);
			seg.setModelType(pcl::SACMODEL_PLANE);
			seg.setMethodType(pcl::SAC_RANSAC);
			seg.setMaxIterations(100);
			seg.setDistanceThreshold(0.02);    //�˴������Լ��޸ģ�һ�㱣��Ĭ�ϼ���

			int i = 0, nr_points = (int)cloud_xyz->points.size();
			while (cloud_xyz->points.size() > 0.3 * nr_points)    //�˴���0.3�����޸ģ�һ�㱣��Ĭ�ϼ���
			{
				// Segment the largest planar component from the remaining cloud
				seg.setInputCloud(cloud_xyz);
				seg.segment(*inliers, *coefficients);
				if (inliers->indices.size() == 0)
				{
					//std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
					
					QMessageBox::information(this,
						QString::fromLocal8Bit("��ʾ��Ϣ"),
						"Could not estimate a planar model for the given dataset.");
					break;
				}

				// Extract the planar inliers from the input cloud
				pcl::ExtractIndices<pcl::PointXYZ> extract;
				extract.setInputCloud(cloud_xyz);
				extract.setIndices(inliers);
				extract.setNegative(false);

				// Get the points associated with the planar surface
				extract.filter(*cloud_plane);
				//std::cout << "PointCloud representing the planar component: " << cloud_plane->points.size() << " data points." << std::endl;
			   /* QMessageBox::information(this,
					QString::fromLocal8Bit("��ʾ��Ϣ"),
					"PointCloud representing the planar component: " + QString::number(cloud_plane->points.size(),10) +" data points.");
				*/
				for (int i = 0; i <cloud_plane->points.size(); i++)
				{

					/*QMessageBox::information(this,
						QString::fromLocal8Bit("��ʾ��Ϣ"),
						QString::fromLocal8Bit("д��������"));
*/
					if (rand() % 100 < 30)            //ƽ�����Ϊ70%
					{
						fout << cloud_plane->points[i].x << " " << cloud_plane->points[i].y << " " << cloud_plane->points[i].z << endl;
						point_num++;
					}
				}

				// Remove the planar inliers, extract the rest
				extract.setNegative(true);
				extract.filter(*cloud_f);
				*cloud_xyz = *cloud_f;
			}
			for (int i = 0; i <cloud_xyz->points.size(); i++)
			{

				/*QMessageBox::information(this,
					QString::fromLocal8Bit("��ʾ��Ϣ"),
					QString::fromLocal8Bit("��һ��д��������"));*/
				if (rand() % 100 < 70)            //����Ϊ30%
				{
					fout << cloud_xyz->points[i].x << " " << cloud_xyz->points[i].y << " " << cloud_xyz->points[i].z << endl;
					point_num++;
				}
			}
			fout << point_num<<"����"<<endl;
			fout.close();
			consoleLog(QString::fromLocal8Bit("��"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].filename.c_str()),
				QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed() / 1000.0) + QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud.cloud->points.size()));
		}
	}
}



/*****    ������ȡ�˵�����ʵ��   *****/

void QtApplication::Boundary()
{
	QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
	int selected_item_count = ui.tree_PointCloud->selectedItems().size();
	if (selected_item_count == 0)
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ѡ��һ�������ļ�"));
		return;
	}
	else
	{

		for (int i = 0; i != selected_item_count; i++)
		{
			timer.restart();//��ʼ��ʱ
			int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
			
			pcl::PointXYZ point;
			cloud_xyz.reset(new pcl::PointCloud<pcl::PointXYZ>);
			timer.restart();//��ʼ��ʱ
			//��mycloud.cloud������XYZRGBA�ĵ��ƣ�ת����cloud_xyz��ֻ����XYZ�ĵ��ƣ�
			//int number = mycloud_vec[cloud_id].cloud->size();
			for (size_t i = 0; i != mycloud_vec[cloud_id].cloud->size(); i++)
			{
				point.x = mycloud_vec[cloud_id].cloud->points[i].x;
				point.y = mycloud_vec[cloud_id].cloud->points[i].y;
				point.z = mycloud_vec[cloud_id].cloud->points[i].z;
				cloud_xyz->push_back(point);//���������ӽ�cloud_xyz������
			}
			if (!cloud_xyz)
			{
				return;
			}

			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_b(new pcl::PointCloud<pcl::PointXYZ>);
			//pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
			//cloud = mycloud_vec[cloud_id].cloud;
			//pcl::io::loadPCDFile("D:\\12.pcd", *cloud);
			//���㷨��
			pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normEst;
			pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
			pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
			pcl::search::KdTree<pcl::PointXYZ>::Ptr tree2(new pcl::search::KdTree<pcl::PointXYZ>);
			tree->setInputCloud(cloud_xyz);
			normEst.setInputCloud(cloud_xyz);
			normEst.setSearchMethod(tree);
			normEst.setKSearch(20);
			normEst.compute(*normals);
			//�жϱ�Ե��
			pcl::PointCloud<pcl::Boundary> boundaries;
			pcl::BoundaryEstimation<pcl::PointXYZ, pcl::Normal, pcl::Boundary> boundEst;
			tree2->setInputCloud(cloud_xyz);
			boundEst.setInputCloud(cloud_xyz);
			boundEst.setInputNormals(normals);
			boundEst.setSearchMethod(tree2);
			boundEst.setKSearch(20);
			boundEst.setAngleThreshold(M_PI / 2);
			boundEst.compute(boundaries);
			//��ȡ��Ե���������
			cloud_b->width = (int)cloud_xyz->points.size();
			cloud_b->height = 1;
			cloud_b->points.resize(cloud_b->width*cloud_b->height);
			int j = 0;
			for (int i = 0; i < cloud_xyz->points.size(); i++)
			{
				if (boundaries.points[i].boundary_point != 0)
				{
					cloud_b->points[j].x = cloud_xyz->points[i].x;
					cloud_b->points[j].y = cloud_xyz->points[i].y;
					cloud_b->points[j].z = cloud_xyz->points[i].z;
					j++;
				}
				//else if (rand() % 100 < 50)            //����Ϊ50%
				//{
				//	cloud_b->points[j].x = cloud->points[i].x;
				//	cloud_b->points[j].y = cloud->points[i].y;
				//	cloud_b->points[j].z = cloud->points[i].z;
				//	j++;
				//}
				continue;
			}
			cloud_b->width = j;
			cloud_b->points.resize(cloud_b->width*cloud_b->height);
			
			pcl::io::savePCDFile(mycloud_vec[cloud_id].dirname + "boundary.pcd", *cloud_b);
			
			
			
					
			mycloud.cloud.reset(new PointCloudT); // Reset cloud
			pcl::copyPointCloud(*cloud_b, *mycloud.cloud);//ת�����Ƹ�ʽ	
			mycloud.filename = mycloud_vec[cloud_id].dirname + "boundary.pcd";
			mycloud.dirname  = mycloud_vec[cloud_id].dirname;
			mycloud.subname  = "boundary.pcd";
			setCloudColor(255,255,0);
			//pcl::io::loadOBJFile(mycloud_vec[cloud_id].dirname + "boundary.pcd", *(mycloud.cloud));
			
			mycloud_vec.push_back(mycloud);


			consoleLog(QString::fromLocal8Bit("��ȡ�����߽�"),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].subname.c_str()),
				QString::fromLocal8Bit(mycloud_vec[cloud_id].filename.c_str()),
				QString::fromLocal8Bit("Time cost: ") + QString("%1s").arg(timer.elapsed() / 1000.0) + QString::fromLocal8Bit(" , Points: ") + QString::number(mycloud.cloud->points.size()));


			//������Դ������
			QTreeWidgetItem *cloudName = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit(mycloud.subname.c_str()));
			cloudName->setIcon(0, QIcon(":/Icon/Resources/Icon/Icon.png"));//������Դ��ͼ��
			ui.tree_PointCloud->addTopLevelItem(cloudName);


			//setWindowTitle(filename + " - CloudViewer"); //���±���

			total_points += mycloud.cloud->points.size();		
		}
		setPropertyTable(mycloud_vec.size(), 0, total_points, "255 255 255");
		showPointcloudAdd();  //������ͼ����
	}

}

/*****    �����˵�����ʵ��   *****/

//��������PCL���ƿ����
void QtApplication::Help()
{
	QDesktopServices::openUrl(QUrl(QLatin1String("http://pointclouds.org/")));

	// �������
	consoleLog(QString::fromLocal8Bit("����"), QString::fromLocal8Bit("PCL���ƿ����"), "http://pointclouds.org/", "");
}

//���ڣ��򿪹��ڽ���
void QtApplication::About()
{
	
	aboutwin->setModal(true);
	aboutwin->show();

	 //�������
	//consoleLog(QString::fromLocal8Bit("����"), "My Graduate Design", "", "DMY");

}



/*****    ��ɫ�����ں���ʵ��   *****/

//RGB��ɫ�����ƴ�С����
void QtApplication::PointSize_SliderReleased()
{
	if (mycloud_vec.size())//��ǰ�е����ļ����޸ĵ��Ƴߴ�
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		int selected_item_count = ui.tree_PointCloud->selectedItems().size();
		// ���δѡ���κε��ƣ������ͼ�����е����е����޸Ĵ�С
		if (selected_item_count == 0){
			for (int i = 0; i != mycloud_vec.size(); i++){
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,
					point_size, "cloud" + QString::number(i).toStdString());
			}

			// �������
			consoleLog(QString::fromLocal8Bit("���õ��ƴ�С"), QString::fromLocal8Bit("���е���"), "Size: " + QString::number(point_size), "");
		}
		else{
			for (int i = 0; i != selected_item_count; i++){
				int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,
					point_size, "cloud" + QString::number(cloud_id).toStdString());
			}
			// �������
			consoleLog(QString::fromLocal8Bit("���õ��ƴ�С"), QString::fromLocal8Bit("��ѡ�е���"), "Size: " + QString::number(point_size), "");
		}
		ui.qvtkWidget->update();
	}
	else//��ǰδ���ص����ļ�
	{
		ui.PointSize_Slider->setValue(1);
		//��ʾ������ӵ����ļ�
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ǰδ�����κε����ļ���"));		
		return;
		
	}
	

}

void QtApplication::RGBsliderReleased()
{
	if (mycloud_vec.size())//��ǰ�е����ļ���������ɫ
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		int selected_item_count = ui.tree_PointCloud->selectedItems().size();
		// ���δѡ���κε��ƣ������ͼ�����е����е��ƽ�����ɫ
		if (selected_item_count == 0)
		{
			for (int i = 0; i != mycloud_vec.size(); i++)
			{
				for (int j = 0; j != mycloud_vec[i].cloud->points.size(); j++)
				{
					mycloud_vec[i].cloud->points[j].r = red;
					mycloud_vec[i].cloud->points[j].g = green;
					mycloud_vec[i].cloud->points[j].b = blue;
				}
			}
			// �������
			consoleLog(QString::fromLocal8Bit("���õ�����ɫ"), QString::fromLocal8Bit("���е���"), QString::number(red) + " " + QString::number(green) + " " + QString::number(blue), "");

		}
		else
		{
			for (int i = 0; i != selected_item_count; i++)
			{
				int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
				for (int j = 0; j != mycloud_vec[cloud_id].cloud->size(); j++)
				{
					mycloud_vec[cloud_id].cloud->points[j].r = red;
					mycloud_vec[cloud_id].cloud->points[j].g = green;
					mycloud_vec[cloud_id].cloud->points[j].b = blue;
				}
			}
			// �������
			consoleLog(QString::fromLocal8Bit("���õ�����ɫ"), QString::fromLocal8Bit("��ѡ�е���"), QString::number(red) + " " + QString::number(green) + " " + QString::number(blue), "");
		}
		showPointcloud();
		//��ɫ���Դ��ڸı�
		ui.table_Properties->setItem(3, 1, new QTableWidgetItem(QString::number(red) + " " + QString::number(green) + " " + QString::number(blue)));
		
	}
	else  //��ǰδ���ص����ļ�
	{
		ui.Red_Slider->setValue(255);	
		ui.Green_Slider->setValue(255);		
		ui.Blue_Slider->setValue(255);
		//��ʾ������ӵ����ļ�
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("��ǰδ�����κε����ļ���"));	
		return;
		
	}	
}

void QtApplication::PointSize_SliderChanged(int value)
{
	point_size = value;
	ui.PointSize_lcdNumber->display(value);

}

void QtApplication::Red_SliderChanged(int value)
{
	red = value;
	ui.Red_lcdNumber->display(value);
}

void QtApplication::Green_SliderChanged(int value)
{
	green = value;
	ui.Green_lcdNumber->display(value);
}

void QtApplication::Blue_SliderChanged(int value)
{
	blue = value;
	ui.Blue_lcdNumber->display(value);
}

//����ϵ��ʾ����
void QtApplication::CordinateChecked(int value)
{
	switch (value)
	{
	case 0:
		viewer->removeCoordinateSystem();  //�Ƴ�����ϵ
		// �������
		consoleLog(QString::fromLocal8Bit("�Ƴ�����ϵ"), QString::fromLocal8Bit("�Ƴ�"), "", "");
		break;
	case 2:
		viewer->addCoordinateSystem();  //�������ϵ
		// �������
		consoleLog(QString::fromLocal8Bit("�������ϵ"), QString::fromLocal8Bit("���"), "", "");
		break;
	}
	ui.qvtkWidget->update();
}

//������ɫ���ú���
void QtApplication::BackgroundChecked(int value)
{
	switch (value)
	{
	case 0:
		viewer->setBackgroundColor(30 / 255.0, 30 / 255.0, 30 / 255.0);
		// �������
		consoleLog(QString::fromLocal8Bit("�ı䱳����ɫ"), QString::fromLocal8Bit("������ɫ"), "30 30 30", "");
		break;
	case 2:
		//��ע�⣺setBackgroundColor()���յ���0-1��double�Ͳ���
		viewer->setBackgroundColor(240 / 255.0, 240 / 255.0, 240 / 255.0);
		// �������
		consoleLog(QString::fromLocal8Bit("�ı䱳����ɫ"), QString::fromLocal8Bit("������ɫ"), "240 240 240", "");
		break;
	}
	ui.qvtkWidget->update();
}



/*****    �����ļ������ں���ʵ��   *****/

//������������ļ���Ӧ����
void QtApplication::dataSelected(QTreeWidgetItem* item, int count)
{
	count = ui.tree_PointCloud->indexOfTopLevelItem(item);  //��ȡitem���к�

	//���Ƚ����е��ƴ�С��Ϊ1
	for (int i = 0; i != mycloud_vec.size(); i++)
	{
		viewer->updatePointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud" + QString::number(i).toStdString());
	}

	//��ȡ��ǰ���Ƶ�RGB,������������Ϣ
	//int cloud_size = mycloud_vec[count].cloud->points.size();
	QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
	int selected_item_count = ui.tree_PointCloud->selectedItems().size();
	long pointsnumber =0;//��¼��ѡ�е����ܵ���
	// ���ѡ�ж�����ƣ����Դ��ڵ�������ʾѡ�еĵ��Ƹ�����;ѡ�е��Ƴߴ���
	for (int i = 0; i != selected_item_count; i++)
	{
		int cloud_id = ui.tree_PointCloud->indexOfTopLevelItem(itemList[i]);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,
			2, "cloud" + QString::number(cloud_id).toStdString());//ѡ�е��Ƴߴ���
		pointsnumber += mycloud_vec[cloud_id].cloud->points.size();
	}
	unsigned int cloud_r = mycloud_vec[count].cloud->points[0].r;
	unsigned int cloud_g = mycloud_vec[count].cloud->points[0].g;
	unsigned int cloud_b = mycloud_vec[count].cloud->points[0].b;
	//�жϵ��Ƶ�ɫ��ɫ�����������Ǻ��Ͻ���
	//bool multi_color = true;
	//unsigned int endpoint = mycloud_vec[count].cloud->points.size();
	//unsigned int bred = mycloud_vec[count].cloud->points[0].r;
	//unsigned int ered = mycloud_vec[count].cloud->points[endpoint - 1].r;
	//if ( bred == ered)
	//	multi_color = false;
	
	//�������Դ�����Ϣ
	setPropertyTable(mycloud_vec.size(), pointsnumber, total_points, QString::number(cloud_r) + " " + QString::number(cloud_g) + " " + QString::number(cloud_b));
	
	ui.qvtkWidget->update();
}
//�Ҽ����������ļ����Ҽ���Ӧ�˵�
void QtApplication::popMenu_DataTree(const QPoint&)
{

	QTreeWidgetItem* curItem = ui.tree_PointCloud->currentItem();//��ȡ��ǰ�һ��ĵ���
	if (curItem == NULL)
	{
		return;//�ڿհ�λ���һ�
	}
	else
	{
		QString curName = curItem->text(0);
		int id = ui.tree_PointCloud->indexOfTopLevelItem(curItem);
		string cloud_id = "cloud" + QString::number(id).toStdString();

		QAction DataHide(QString::fromLocal8Bit("����"), this);
		QAction DataShow(QString::fromLocal8Bit("��ʾ"), this);
		QAction DataDelete(QString::fromLocal8Bit("ɾ��"), this);
		QAction ColorChange(QString::fromLocal8Bit("�޸ĵ�����ɫ"), this);

		connect(&DataHide, &QAction::triggered, this, &QtApplication::dataHide);
		connect(&DataShow, &QAction::triggered, this, &QtApplication::dataShow);
		connect(&DataDelete, &QAction::triggered, this, &QtApplication::dataDelete);
		connect(&ColorChange, &QAction::triggered, this, &QtApplication::dataColorChange);

		QPoint pos;
		QMenu *popMenu = new QMenu(this);		
		popMenu->addAction(&DataHide);
		popMenu->addAction(&DataShow);
		popMenu->addAction(&DataDelete);
		popMenu->addAction(&ColorChange);

		if (mycloud_vec[id].visible == true)
		{
			popMenu->actions()[0]->setVisible(true);
			popMenu->actions()[1]->setVisible(false);
		}
		else
		{
			popMenu->actions()[0]->setVisible(false);
			popMenu->actions()[1]->setVisible(true);
		}
		popMenu->exec(QCursor::pos()); //�ڵ�ǰ���λ����ʾ
	}
	

}
//���ص����ļ�
void QtApplication::dataHide()
{
	if (ui.tree_PointCloud->selectedItems().size())
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		for (int i = 0; i != ui.tree_PointCloud->selectedItems().size(); i++)
		{
			QTreeWidgetItem* curItem = itemList[i];
			QString name = curItem->text(0);
			int id = ui.tree_PointCloud->indexOfTopLevelItem(curItem);
			string cloud_id = "cloud" + QString::number(id).toStdString();
			//QMessageBox::information(this, "cloud_id", QString::fromLocal8Bit(cloud_id.c_str()));
			// ��cloud_id����Ӧ�ĵ������ó�͸��
			viewer->setPointCloudRenderingProperties(pcl::visualization::RenderingProperties::PCL_VISUALIZER_OPACITY, 0.0, cloud_id, 0);
			QColor item_color = QColor(112, 122, 132, 255);
			curItem->setTextColor(0, item_color);
			mycloud_vec[id].visible = false;
		}
		consoleLog(QString::fromLocal8Bit("���ص���"), QString::fromLocal8Bit("��ѡ�е���"), "", "");
		ui.qvtkWidget->update(); //ˢ����ͼ���ڣ�����ʡ��
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("δѡ���κε��ƣ�"));
		return;
	}
}
//��ʾ�����ļ�
void QtApplication::dataShow()
{
	if (ui.tree_PointCloud->selectedItems().size())
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		for (int i = 0; i != ui.tree_PointCloud->selectedItems().size(); i++){
			//QTreeWidgetItem* curItem = ui.dataTree->currentItem();
			QTreeWidgetItem* curItem = itemList[i];
			QString name = curItem->text(0);
			int id = ui.tree_PointCloud->indexOfTopLevelItem(curItem);
			string cloud_id = "cloud" + QString::number(id).toStdString();
			// ��cloud_id����Ӧ�ĵ������ó�͸��
			viewer->setPointCloudRenderingProperties(pcl::visualization::RenderingProperties::PCL_VISUALIZER_OPACITY, 1.0, cloud_id, 0);
			QColor item_color;
			item_color = QColor(0, 0, 0, 255);
			curItem->setTextColor(0, item_color);
			mycloud_vec[id].visible = true;
		}

		// �������
		consoleLog(QString::fromLocal8Bit("��ʾ����"), 
			QString::fromLocal8Bit("��ѡ�е���"), "", "");
		ui.qvtkWidget->update(); //ˢ����ͼ���ڣ�����ʡ��

	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("δѡ���κε��ƣ�"));
		return;

	}

}
//�Ƴ������ļ�
void QtApplication::dataDelete()
{
	if (ui.tree_PointCloud->selectedItems().size())
	{
		QList<QTreeWidgetItem*> itemList = ui.tree_PointCloud->selectedItems();
		//���ŵ����������ı䣬���ѭ������Ҫ����Ϊ�̶���С�� selected_item_count
		int selected_item_count = ui.tree_PointCloud->selectedItems().size();
		for (int i = 0; i != selected_item_count; i++){
			QTreeWidgetItem* curItem = itemList[i];
			QString name = curItem->text(0);
			int id = ui.tree_PointCloud->indexOfTopLevelItem(curItem);
			auto it = mycloud_vec.begin() + ui.tree_PointCloud->indexOfTopLevelItem(curItem);
			// ɾ������֮ǰ����������Ŀ����
			int delete_points = (*it).cloud->points.size();
			it = mycloud_vec.erase(it);
			total_points -= delete_points;

			if (mycloud_vec.size()==1)
			{			
				unsigned int r = mycloud_vec[0].cloud->points[0].r;
				unsigned int g = mycloud_vec[0].cloud->points[0].g;
				unsigned int b = mycloud_vec[0].cloud->points[0].b;
				QString RGB_value = QString::number(r) + " " + QString::number(g) + " " + QString::number(b);
				setPropertyTable(mycloud_vec.size(), mycloud_vec[0].cloud->points.size(), total_points, RGB_value);
			}
			else
			{
				setPropertyTable(mycloud_vec.size(),0, total_points," ");
			}
			

			ui.tree_PointCloud->takeTopLevelItem(ui.tree_PointCloud->indexOfTopLevelItem(curItem));
		}

		// �Ƴ�֮������ӣ����� id �͵������кŲ�һ�µ����
		viewer->removeAllPointClouds();
		for (int i = 0; i != mycloud_vec.size(); i++)
		{
			viewer->addPointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
			viewer->updatePointCloud(mycloud_vec[i].cloud, "cloud" + QString::number(i).toStdString());
		}
		// �������
		consoleLog(QString::fromLocal8Bit("ɾ������"), QString::fromLocal8Bit("��ѡ�е���"), "", "");
		viewer->resetCamera();//��������Ƕ�
		ui.qvtkWidget->update();
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("δѡ���κε��ƣ�"));
		return;
	}

}
//�Ҽ��޸ĵ�����ɫ
void QtApplication::dataColorChange()
{
	if (ui.tree_PointCloud->selectedItems().size())
	{
		PointColorChange();
	}
	else
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ��Ϣ"),
			QString::fromLocal8Bit("δѡ���κε��ƣ�"));
		return;
	}
}



/*****    ������¼�����ں���ʵ��   *****/

//������¼���ڵ��һ���Ӧ�˵�
void QtApplication::popMenu_Console(const QPoint&)
{
	QAction ClearConsoleTable(QString::fromLocal8Bit("��ղ�����¼"), this);
	QAction EnableConsoleTable(QString::fromLocal8Bit("���ò�����¼"), this);
	QAction DisableConsoleTable(QString::fromLocal8Bit("���ò�����¼"), this);

	connect(&ClearConsoleTable, &QAction::triggered, this, &QtApplication::clearConsole);
	connect(&EnableConsoleTable, &QAction::triggered, this, &QtApplication::enableConsole);
	connect(&DisableConsoleTable, &QAction::triggered, this, &QtApplication::disableConsole);

	QPoint pos;
	QMenu *popMenu = new QMenu(this);//����һ���Ҽ������˵�
	//QMenu popMenu(ui.table_Console);
	popMenu->addAction(&ClearConsoleTable);
	popMenu->addAction(&EnableConsoleTable);
	popMenu->addAction(&DisableConsoleTable);

	if (enable_console == true)
	{ 
		popMenu->actions()[1]->setVisible(false);
		popMenu->actions()[2]->setVisible(true);
	}
	else
	{
		popMenu->actions()[1]->setVisible(true);
		popMenu->actions()[2]->setVisible(false);

	}
	popMenu->exec(QCursor::pos()); //�ڵ�ǰ���λ����ʾ
}
// ��ղ�����¼
void QtApplication::clearConsole(){
	ui.table_Console->clearContents();
	ui.table_Console->setRowCount(0);
}
// ���ò�����¼
void QtApplication::enableConsole(){
	
	enable_console = true;
}
// ���ò�����¼
void QtApplication::disableConsole(){
	clearConsole();//������¼����ǰ��ղ�����¼
	enable_console = false;
}



