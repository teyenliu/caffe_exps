//--------------------------------------【程式說明】-------------------------------------------
//		程式說明：《OpenCV3程式設計入門》OpenCV2版書本配套範例程式19
//		程式描述：基礎圖像容器Mat類別的使用
//		開發測試所用操作系統： Windows 7 64bit
//		開發測試所用IDE版本：Visual Studio 2010
//		開發測試所用OpenCV版本：	2.4.9
//		2014年06月 Created by @淺墨_毛星云
//		2014年12月 Revised by @淺墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【頭文件、命名空間包含部分】---------------------------
//          描述：包含程式所使用的頭文件和命名空間
//-----------------------------------------------------------------------------------------------
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;



//-----------------------------【ShowHelpText( )函數】--------------------------------------
//		描述：輸出說明訊息
//-------------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//輸出歡迎訊息和OpenCV版本
	printf("\n\n\t\t\t非常感謝購買《OpenCV3程式設計入門》一書！\n");
	printf("\n\n\t\t\t此為本書OpenCV2版的第19個配套範例程式\n");
	printf("\n\n\t\t\t   現在使用的OpenCV版本為：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");

	//輸出一些說明訊息
	printf("\n\n\n\t歡迎來到【基本圖像容器-Mat類別】範例程式~\n\n"); 	
	printf("\n\n\t程式說明：\n\n\t此範例程式用於示範Mat類別的格式化輸出功能，輸出風格可為：");
	printf("\n\n\n\t【1】OpenCV預設風格");
	printf("\n\n\t【2】Python風格");
	printf("\n\n\t【3】逗號分隔風格");
	printf("\n\n\t【4】Numpy風格");
	printf("\n\n\t【5】C語言風格\n\n");
	printf("\n  --------------------------------------------------------------------------\n");



}


//--------------------------------------【main( )函數】-----------------------------------------
//          描述：控制臺應用程式的入口函數，我們的程式從這里開始執行
//-----------------------------------------------------------------------------------------------
int main(int,char**)
{
	//改變控制臺的前景色和背景色
	system("color 8F"); 

	//顯示說明文字
	ShowHelpText();

	Mat I = Mat::eye(4, 4, CV_64F);
	I.at<double>(1,1) = CV_PI;
	cout << "\nI = " << I << ";\n" << endl;

	Mat r = Mat(10, 1, CV_8UC3);
	randu(r, Scalar::all(0), Scalar::all(255));

	cout << "r (OpenCV預設風格) = " << r << ";" << endl << endl;
	cout << "r (Python風格) = " << format(r,"python") << ";" << endl << endl;
	cout << "r (Numpy風格) = " << format(r,"numpy") << ";" << endl << endl;
	cout << "r (逗號分隔風格) = " << format(r,"csv") << ";" << endl<< endl;
	cout << "r (C語言風格) = " << format(r,"C") << ";" << endl << endl;

	Point2f p(6, 2);
	cout << "【2維點】p = " << p << ";\n" << endl;

	Point3f p3f(8, 2, 0);
	cout << "【3維點】p3f = " << p3f << ";\n" << endl;

	vector<float> v;
	v.push_back(3);
	v.push_back(5);
	v.push_back(7);

	cout << "【基于Mat的vector】shortvec = " << Mat(v) << ";\n"<<endl;

	vector<Point2f> points(20);
	for (size_t i = 0; i < points.size(); ++i)
		points[i] = Point2f((float)(i * 5), (float)(i % 7));

	cout << "【二維點向量】points = " << points<<";";

	getchar();//按任意鍵退出

	return 0;


}

