#ifndef QTAPPLICATION_H
#define QTAPPLICATION_H

#include <vtkAutoInit.h> 
#include <vtkRenderWindow.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);



#include "MyCloud.h"
#include "ui_qtapplication.h"
#include "Tools.h"
#include "AboutWin.h"
#include "SimplifyWin.h"
#include "PointSelected.h"

//��׼C++ͷ�ļ����ø�ʽ
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include <boost/concept_check.hpp>
#include <pcl/ModelCoefficients.h>

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h>
#include <pcl/io/obj_io.h>

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/common/common.h>
//#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/common/common.h>
#include <pcl/visualization/point_picking_event.h>

#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>

#include <pcl/features/normal_3d.h>
#include <pcl/features/boundary.h>

#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/search/kdtree.h>
//#include <pcl/octree/octree_pointcloud.h>
//#include <pcl/octree/octree.h>

#include <pcl/surface/gp3.h>

#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>

#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>


#include <QtWidgets/QMainWindow>//contains UI elements
#include <QString>
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "QVTKWidget.h"
#include <QTextEdit>
#include <QTime>
#include <QMouseEvent> 
#include <QDesktopServices> 
#include <QUrl>


//improves readabily
typedef pcl::PointXYZRGBA PointT;
//brief A point structure representing Euclidean xyz coordinates, and the RGBA color
typedef pcl::PointCloud<PointT> PointCloudT;
//brief PointCloud represents the base class in PCL for storing collections of 3D points




class QtApplication : public QMainWindow
{
	Q_OBJECT

public:
	QtApplication(QWidget *parent = 0);
	~QtApplication();
	static void SelectPoint_callback(const pcl::visualization::PointPickingEvent& event, void* args);//��̬�ص�����
	static QtApplication *pThis;//��̬����ָ��
	void PointPickingShow(pcl::PointXYZRGBA current_point);
	boost::mutex cloud_mutex;
	struct callback_args		// ���ڽ��������ݸ��ص������Ľṹ
	{
		PointCloudT::Ptr clicked_points_3d;
		pcl::visualization::PCLVisualizer::Ptr viewerPtr;
	};
private:
	Ui::QtApplicationClass ui;

	// Mutex: //


	//�������ݴ洢
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_xyz;//ֻ��XYZ����ĵ�ĵ���
	
	MyCloud mycloud;//PointXZYRGBA���͵ĵ���
	std::vector<MyCloud> mycloud_vec;  //��������
	
	//pcl::visualization::PCLVisualizer::Ptr viewer = new pcl::visualization::PCLVisualizer("viewer");
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	//pcl::visualization::PCLVisualizer::Ptr viewer;

	long total_points = 0;//�������е����ܵ���

	unsigned int red = 255;
	unsigned int green = 255;
	unsigned int blue = 255;
	unsigned int point_size = 2;
	std::vector<int> randomcolor;
	std::vector<int> bgcolor;
	
	QTime timer;//��ʱ��,��¼ĳ��������ִ�е�ʱ��
	bool enable_console = true; // ������¼�Ŀ���״̬
	
	//�Ӵ���ʵ����
	AboutWin *aboutwin = new AboutWin(this);
	SimplifyWin *simplifywin = new SimplifyWin(this);
	PointSelected *pointselected = new PointSelected(this);


	/***** һЩ��Ҫ�����õķ��� ***/
	void initial();//��ʼ��vtk����
	void showPointcloud();  //��ʾ����
	void showPointcloudAdd();  //��Ӹ�viewer����ʾ����

	void setCloudColor(unsigned int r, unsigned int g, unsigned int b);//���õ�����ɫ
	void setA(unsigned int a);//���õ���͸����
	
	void setPropertyTable(int clouds,int points,int totalpoints,QString RGB);//�������Թ�����
	void setConsoleTable();//���ò�����¼������
	void setRGBDock();//����RGBDock����
	void consoleLog(QString operation, QString subname, QString filename, QString note);//������¼��д����
	
	pcl::PointCloud<pcl::PointXYZ>::Ptr transformPointCloud(MyCloud cloud_RGBA);//��MyCloud(PointCloud_RGBA)ת����PointCloud_XYZ
	void saveResult(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_result);
	


private slots:

	/***** ���²ۺ�����UI�����ϲ˵��������� *****/
	// �ļ��˵���
	void Open();
	void Add();
	void Save();
	void Savemulti(QString savename);
	void Clear();
	void Exit();
	// ��ͼ�˵���
	void HideDataDock();
	void HidePropertiesDock();
	void HideConsoleDock();
	void HideRGBDock();		
	// ��ʾ�˵���
	void RandomColor();
	void PointColorChange();
	void BackgroundColorChange();
	void MainView();//����ͼ
	void LeftView();//����ͼ
	void TopView();//����ͼ
	//��άͼ�β˵���
	void CreateSphere();//����һ����ά����

	// ���Ƽ򻯲˵���
	void Simplify();
	void ReceiveData(QString rate);
	void Simplify2(int cloud_id,int rate);
	void BoundingBox();
	// ������ȡ�˵���
	void SelectPoint();
	void Boundary();
    // ��ά�ؽ��˵���
	void Surface();//���߹��ơ������ؽ���������Ƭ��ʾ
	void Wireframe();//���߹��ơ������ؽ��������߿���ʾ
	// �����˵���
	void Help();
	void About();

	//�������˵���
	//void Cordinate();


	/***** ��ɫ������ *****/
	// ���õ�����ɫ����С
	//void radomcolorBtnPressed();
	void RGBsliderReleased();
	void PointSize_SliderReleased();
	void PointSize_SliderChanged(int value);
	void Red_SliderChanged(int value);
	void Green_SliderChanged(int value);
	void Blue_SliderChanged(int value);

	// ����ϵ��������ɫ��ѡ��
	void CordinateChecked(int value);
	void BackgroundChecked(int value);
	
	
	/***** �����ļ������� *****/
	// ������������ļ�
	void dataSelected(QTreeWidgetItem*, int);
	// �Ҽ����������ļ����Ҽ���Ӧ�˵�
	void popMenu_DataTree(const QPoint&);
	void dataHide();
	void dataShow();
	void dataDelete();
	void dataColorChange();
	
	/***** ������¼������ *****/
	//�Ҽ���Ӧ�˵�
	void popMenu_Console(const QPoint&);
	void clearConsole();//��ղ�����¼
	void enableConsole();
	void disableConsole();

};

#endif // QTAPPLICATION_H
