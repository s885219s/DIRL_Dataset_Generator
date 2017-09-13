#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <typeinfo>
#include <windows.h>
#include <ShellScalingAPI.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <fstream>

using namespace std;
using namespace cv;
using namespace chrono;

float screen_width = GetSystemMetrics(SM_CXSCREEN);
float screen_height = GetSystemMetrics(SM_CYSCREEN);
bool mouse = FALSE;

string user_name = "0100";
float subject_distance = 550.0;
float monitor_width = 1210.0;
float monitor_height = 680.0;
string user_pose = "0";
int width_points = 9;
int width_range = 40;
int height_points = 7;
int height_range = 30;
int max_count = 100;

vector<string> get_arguments(int argc, char **argv)
{

	vector<string> arguments;

	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(string(argv[i]));
	}
	return arguments;
}
void takePhotoParameters(vector<string> &arguments) {
	for (size_t i = 1; i < arguments.size(); ++i) {
		if (arguments[i].compare("-un") == 0)
			user_name = arguments[i + 1];
		if (arguments[i].compare("-d") == 0) {
			stringstream data(arguments[i + 1]);
			data >> subject_distance;
		}
		if (arguments[i].compare("-w") == 0) {
			stringstream data(arguments[i + 1]);
			data >> monitor_width;
		}
		if (arguments[i].compare("-h") == 0) {
			stringstream data(arguments[i + 1]);
			data >> monitor_height;
		}
		if (arguments[i].compare("-p") == 0)
			user_pose = arguments[i + 1];
		if (arguments[i].compare("-wp") == 0) {
			stringstream data(arguments[i + 1]);
			data >> width_points;
		}
		if (arguments[i].compare("-wr") == 0) {
			stringstream data(arguments[i + 1]);
			data >> width_range;
		}
		if (arguments[i].compare("-hp") == 0) {
			stringstream data(arguments[i + 1]);
			data >> height_points;
		}
		if (arguments[i].compare("-hr") == 0) {
			stringstream data(arguments[i + 1]);
			data >> height_range;
		}
		if (arguments[i].compare("-c") == 0) {
			stringstream data(arguments[i + 1]);
			data >> max_count;
		}
	}
}
void take_photo(int event,int x ,int y, int flags, void* param) {
	if (event == CV_EVENT_LBUTTONDOWN) {
		mouse = TRUE;
	}
}
int random_num(int i) { return rand() % i; }

int main(int argc, char *argv[])
{
	vector<string> arguments = get_arguments(argc, argv);
	takePhotoParameters(arguments);
	cout << "username: " << user_name << " pose: " << user_pose << endl;
	cout << "mw: " << monitor_width << " mh: " << monitor_height << endl;
	Mat image;
	Mat photo;
	int i = 0;
	int count = -1;
	int key = 0;
	vector<int> rand_test_set_one;
	vector<int> rand_test_set_two;
	int start_coordinate[2] = { 0 };
	string photo_name = "";

	int total_count = width_points*height_points;
	int major_count = width_points * 3 + (height_points - 3) * 3;
	int minor_count = total_count - major_count;
	int temp = 0;
	//vector<vector<float>> gaze_points= vector<vector<float>>();
	//vector<int> test_set_y = {0};
	float ppi = screen_width / monitor_width;
	float eye_distance = subject_distance * ppi;
	

	VideoWriter writer;

	//cout <<"total : " <<total_count << "\n major : " << major_count << "\n minor : " << minor_count << endl;
	wstring stemp = wstring(user_name.begin(), user_name.end());
	LPCWSTR sw = stemp.c_str();
	if (CreateDirectory(sw, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		cout << "Create new folder!!" << endl;
	}
	else
	{
		cout << "The folder is exist!!" << endl;
		exit(-1);
	}


	vector<int> compression_params;
	//compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(3);	
	

	cout << "x:" << to_string(screen_width) << "y:" << to_string(screen_height) << "\n";
	Mat background(screen_height, screen_width, CV_8UC3, Scalar(0, 0, 0));
	
	namedWindow("GAZE POSITION TESTING", CV_WINDOW_NORMAL);
	setWindowProperty("GAZE POSITION TESTING", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	line(background, Point(screen_width / 2 - 20, screen_height / 2 - 20), Point(screen_width / 2  + 20, screen_height / 2 + 20), Scalar(255, 255, 255), 3);
	line(background, Point(screen_width / 2 + 20, screen_height / 2  - 20), Point(screen_width / 2 - 20, screen_height / 2 + 20), Scalar(255, 255, 255), 3);
	circle(background, Point(screen_width / 2, screen_height / 2), 5, Scalar(0, 0, 255), -1);
	//rectangle(background, Point(0,0), Point(screen_width-2,screen_height-2), Scalar(255, 255, 255), 2);
	imshow("GAZE POSITION TESTING", background);
	line(background, Point(screen_width / 2 - 20, screen_height / 2 - 20), Point(screen_width / 2 + 20, screen_height / 2 + 20), Scalar(0, 0, 0), 3);
	line(background, Point(screen_width / 2 + 20, screen_height / 2 - 20), Point(screen_width / 2 - 20, screen_height / 2 + 20), Scalar(0, 0, 0), 3);
	circle(background, Point(screen_width / 2, screen_height / 2), 5, Scalar(0, 0, 0), -1);


	srand(unsigned(time(NULL)));
	for (i = 0 ; i < max_count; i++) rand_test_set_one.push_back(i);
	random_shuffle(rand_test_set_one.begin(), rand_test_set_one.end());
	random_shuffle(rand_test_set_one.begin(), rand_test_set_one.end(), random_num);
	
	float **gaze_points = new float*[max_count];
	for(int i = 0; i < max_count;i++)
		gaze_points[i] = new float[4];

    //63 pixel point
	int gaze_n = 0;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			int v = i * 10 - 30;
			int h = j * 10 - 40;
			gaze_points[gaze_n][0] = v;
			gaze_points[gaze_n][1] = h;
			float angleX = tan(gaze_points[gaze_n][1] * M_PI / 180);
			float angleY = tan(gaze_points[gaze_n][0] * M_PI / 180);
			float screen_distance_pixelsX = angleX * eye_distance;
			float screen_distance_pixelsY = angleY * eye_distance;
			if (screen_distance_pixelsX > 0) gaze_points[gaze_n][3] = (int)(screen_distance_pixelsX + 0.5f);
			else if (screen_distance_pixelsX < 0) gaze_points[gaze_n][3] = (int)(screen_distance_pixelsX - 0.5f);
			if (screen_distance_pixelsY > 0) gaze_points[gaze_n][2] = (int)(screen_distance_pixelsY + 0.5f);
			else if (screen_distance_pixelsY < 0) gaze_points[gaze_n][2] = (int)(screen_distance_pixelsY - 0.5f);
			gaze_n++;
		}
	}
	// 37 random
	for (int i = 0; i < max_count - 63; i++) {
		int v = rand() % 40 - 20;
		int h = rand() % 60 - 30;
		gaze_points[gaze_n][0] = v;
		gaze_points[gaze_n][1] = h;
		float angleX = tan(gaze_points[gaze_n][1] * M_PI / 180);
		float angleY = tan(gaze_points[gaze_n][0] * M_PI / 180);
		float screen_distance_pixelsX = angleX * eye_distance;
		float screen_distance_pixelsY = angleY * eye_distance;
		if (screen_distance_pixelsX > 0) gaze_points[gaze_n][3] = (int)(screen_distance_pixelsX + 0.5f);
		else if (screen_distance_pixelsX < 0) gaze_points[gaze_n][3] = (int)(screen_distance_pixelsX - 0.5f);
		if (screen_distance_pixelsY > 0) gaze_points[gaze_n][2] = (int)(screen_distance_pixelsY + 0.5f);
		else if (screen_distance_pixelsY < 0) gaze_points[gaze_n][2] = (int)(screen_distance_pixelsY - 0.5f);
		gaze_n++;
	}
	if (gaze_n != (max_count)) {
		return 0;
	}
	
	vector<int> exp_seq;
	for (i = 0; i < max_count; i++) {
		exp_seq.push_back(i);
	}
	random_shuffle(exp_seq.begin(), exp_seq.end());

	
	VideoCapture cap;    
	cap.open(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	cap.set(CV_CAP_PROP_EXPOSURE, -7.5);//曝光度
	Size videoSize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	writer.open(user_name + "\\" + user_name + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);	
	namedWindow("WEBCAM", CV_WINDOW_NORMAL);
	setWindowProperty("WEBCAM", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	setMouseCallback("WEBCAM", take_photo, NULL);
	milliseconds vms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	cap.read(image);
	waitKey(33);
	cap.read(image);
	waitKey(33);
	cap.read(image);
	waitKey(33);
	ofstream outputfile(user_name + "\\" + user_name + "_log.txt");
	outputfile << "timestamp,event" << endl;

	outputfile << to_string(vms.count()) << "," << "start_recording" << endl;
	vms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	
	//CV_CAP_PROP_BRIGHTNESS 影像亮度
	//CV_CAP_PROP_CONTRAST 影像對比度
	//CV_CAP_PROP_SATURATION 影像飽和度
	while (cap.isOpened())   
	{ 
		int cur_setting = 0;
		if (count != -1)
			cur_setting = exp_seq[count];
		cap.read(image);
		if (image.empty())
			return(0);
		if (count == -1) {
			line(image, Point(0, screen_height / 2), Point(screen_width, screen_height / 2), Scalar(0, 0, 255), 2);
			line(image, Point(screen_width / 2, 0), Point(screen_width / 2, screen_height), Scalar(0, 0, 255), 2);
		}
		writer.write(image);
		vms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

		// show captured image to the screen 2
		imshow("WEBCAM", image);

		if (mouse) {
			milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			if (count == -1) {
				photo_name = user_name + "_" + user_pose + "P_0H_0V" + ".png";
				imwrite(user_name + "\\" + "Start_" + photo_name, image, compression_params);
				// move to screen 1
				moveWindow("WEBCAM", -1920, 0);
			}
			else {
				cout << "Save: " << photo_name << endl;
				cout << "ms:" << to_string(ms.count()) << endl;
				outputfile << to_string(ms.count()) << ",saving_img(" << user_name + "\\" + photo_name << ")" << endl;
				cout << count << endl;
				int true_hor = int(gaze_points[cur_setting][1])*(-1);
				//cout << gaze_points[cur_setting][1] << ',' << true_hor << endl;
				photo_name = user_name + "_" + user_pose + "P_" + to_string(true_hor) + "H_" + to_string(int(gaze_points[cur_setting][0])) + "V" + ".png";
				imwrite(user_name + "\\" + photo_name, image, compression_params);
				ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
				outputfile << to_string(ms.count()) << ",saving_finish" << endl;
			}
			// set next points 
			count++;
			if (count == max_count)
				break;
			cur_setting = exp_seq[count];
			//background.setTo(Scalar(0, 0, 0));

			int x = gaze_points[cur_setting][3];
			int y = gaze_points[cur_setting][2];
			line(background, Point(screen_width / 2 + x - 20, screen_height / 2 - y - 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y + 20), Scalar(255, 255, 255), 3);
			line(background, Point(screen_width / 2 + x - 20, screen_height / 2 - y + 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y - 20), Scalar(255, 255, 255), 3);
			circle(background, Point(screen_width / 2 + x, screen_height / 2 - y), 5, Scalar(0, 0, 255), -1);
			//rectangle(bg, Point(0, 0), Point(screen_width - 2, screen_height - 2), Scalar(255, 255, 255), 2);
			imshow("GAZE POSITION TESTING", background);
			line(background, Point(screen_width / 2 + x - 20, screen_height / 2 - y - 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y + 20), Scalar(0, 0, 0), 3);
			line(background, Point(screen_width / 2 + x - 20, screen_height / 2 - y + 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y - 20), Scalar(0, 0, 0), 3);
			circle(background, Point(screen_width / 2 + x, screen_height / 2 - y), 5, Scalar(0, 0, 0), -1);
			ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			outputfile << to_string(ms.count()) << ",showing_GP" << endl;
			mouse = FALSE;
		}
		if (waitKey(33) == 27 ) {
			break;
		}
	}
	//write file
	vms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	outputfile << to_string(vms.count()) << "," << "end_recording" << endl;
	
	// close file
	outputfile.close();
	
	for (int i = 0; i < max_count; ++i) {
		delete[] gaze_points[i];
	}
	delete[] gaze_points;
	return 0;
}