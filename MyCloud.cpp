#include "MyCloud.h"


MyCloud::MyCloud()
{
}


MyCloud::~MyCloud()
{
}

void MyCloud::initial()
{
	cloud.reset(new PointCloudT);
	cloud->resize(0);//RGBA���Ƴ�ʼ������height=0�������
	filename = "";
	subname = "";
	dirname = "E:\\Date\\PointCloud\\";
}