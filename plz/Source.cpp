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

using namespace std;
using namespace cv;

float screen_width = GetSystemMetrics(SM_CXSCREEN);
float screen_height = GetSystemMetrics(SM_CYSCREEN);
//cout << "x:"<< to_string(x) << "y:" << to_string(y) << "\n";

void show_out_point(Mat bg,int paramX, int paramY,float eye_distance, int x_points,int x_range,int y_points, int y_range) {
	int x = 0;
	int y = 0;
	double angleX = tan((paramX - (x_points - 1) / 2) * (x_range * 2 / (x_points - 1)) * M_PI / 180);
	double angleY = tan((paramY - (y_points - 1) / 2) * (y_range * 2 / (y_points - 1)) * M_PI / 180);
	double screen_distance_pixelsX = angleX * eye_distance;
	double screen_distance_pixelsY = angleY * eye_distance;
	if (screen_distance_pixelsX > 0) x = (int)(screen_distance_pixelsX + 0.5f);
	else if (screen_distance_pixelsX < 0) x = (int)(screen_distance_pixelsX - 0.5f);
	
	if (screen_distance_pixelsY > 0) y = (int)(screen_distance_pixelsY + 0.5f);
	else if (screen_distance_pixelsY < 0) y = (int)(screen_distance_pixelsY - 0.5f);

	cout <<"Nextout: " <<"x: " << (paramX- (x_points - 1) / 2) * (x_range * 2 / (x_points - 1)) << "y: " << (paramY - (y_points - 1) / 2) * (y_range * 2 / (y_points - 1)) << '\n';

	bg.setTo(Scalar(0, 0, 0));
	line(bg, Point(screen_width / 2 + x - 20, screen_height / 2 - y - 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y + 20), Scalar(255, 255, 255), 3);
	line(bg, Point(screen_width / 2 + x - 20, screen_height / 2 - y + 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y - 20), Scalar(255, 255, 255), 3);
	//rectangle(bg, Point(0, 0), Point(screen_width - 2, screen_height - 2), Scalar(255, 255, 255), 2);
	//circle(bg, Point(screen_width / 2 + x, screen_height / 2 - y), 10, Scalar(255, 255, 255), -1);
	imshow("GAZE POSITION TESTING", bg);
}
void show_in_point(Mat bg,int* start,int paramX, int paramY, float eye_distance, int x_points, int x_range, int y_points, int y_range) {
	int x = 0;
	int y = 0;
	double angleX = tan((start[0] + paramX - (x_points - 1) / 2) * (x_range * 2 / (x_points - 1)) * M_PI / 180);
	double angleY = tan((start[1] + paramY - (y_points - 1) / 2) * (y_range * 2 / (y_points - 1)) * M_PI / 180);
	double screen_distance_pixelsX = angleX * eye_distance;
	double screen_distance_pixelsY = angleY * eye_distance;
	if (screen_distance_pixelsX > 0) x = (int)(screen_distance_pixelsX + 0.5f);
	else if (screen_distance_pixelsX < 0) x = (int)(screen_distance_pixelsX - 0.5f);

	if (screen_distance_pixelsY > 0) y = (int)(screen_distance_pixelsY + 0.5f);
	else if (screen_distance_pixelsY < 0) y = (int)(screen_distance_pixelsY - 0.5f);
	//cout << start[0] << " ," << start[1] << '\n';
	cout << "Nextin: " << "x: " << (start[0]+ paramX - (x_points - 1) / 2) *(x_range * 2 / (x_points - 1)) << "y: " << (start[1]+paramY - (y_points - 1) / 2) * (y_range * 2 / (y_points - 1)) << '\n';

	bg.setTo(Scalar(0, 0, 0));
	line(bg, Point(screen_width / 2 + x - 20, screen_height / 2 - y - 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y + 20), Scalar(255, 255, 255), 3);
	line(bg, Point(screen_width / 2 + x - 20, screen_height / 2 - y + 20), Point(screen_width / 2 + x + 20, screen_height / 2 - y - 20), Scalar(255, 255, 255), 3);
	//rectangle(bg, Point(0, 0), Point(screen_width - 2, screen_height - 2), Scalar(255, 255, 255), 2);
	//circle(bg, Point(screen_width / 2 + x, screen_height / 2 - y), 10, Scalar(255, 255, 255), -1);
	imshow("GAZE POSITION TESTING", bg);

}

int random_num(int i) { return rand() % i; }

int main(int argc, char *argv[])
{
	//cout << argv[1] << '\n';
	Mat image;
	Mat photo;
	int i = 0;
	int key = 0;
	int count = 0;
	vector<int> rand_test_set_one;
	vector<int> rand_test_set_two;
	int start_coordinate[2] = { 0 };
	string photo_name = "";
	string user_name = argv[1];
	float subject_distance = atoi(argv[2]);
	float monitor_width = atoi(argv[3]);
    float monitor_height = atoi(argv[4]);
	string user_pose = argv[5];
	int width_points = atoi(argv[6]);
	int width_range = atoi(argv[7]);
	int height_points = atoi(argv[8]);
	int height_range = atoi(argv[9]);
	
	int test_set_x = (width_points - 1) / 2;
	int test_set_y = (height_points - 1) / 2;
	float ppi = screen_width / monitor_width;
	float eye_distance = subject_distance * ppi;
	int total_count = width_points*height_points;
	int major_count = width_points * 3 + (height_points - 3) * 3;
	int minor_count = total_count - major_count;
	cout <<"total : " <<total_count << "\n major : " << major_count << "\n minor : " << minor_count << endl;
	/*
	for (i = 0; i < argc; i++) {
		cout << argv[i] << endl;
	}
	cout << eye_distance << endl;
	*/
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
	
	Mat background(screen_height, screen_width, CV_8UC3, Scalar(0, 0, 0));
	namedWindow("GAZE POSITION TESTING", CV_WINDOW_NORMAL);
	setWindowProperty("GAZE POSITION TESTING", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	line(background, Point(screen_width / 2 - 20, screen_height / 2 - 20), Point(screen_width / 2  + 20, screen_height / 2 + 20), Scalar(255, 255, 255), 3);
	line(background, Point(screen_width / 2 + 20, screen_height / 2  - 20), Point(screen_width / 2 - 20, screen_height / 2 + 20), Scalar(255, 255, 255), 3);
	//rectangle(background, Point(0,0), Point(screen_width-2,screen_height-2), Scalar(255, 255, 255), 2);
	//circle(background, Point(screen_width/2, screen_height/2), 10, Scalar(255, 255, 255), -1);
	imshow("GAZE POSITION TESTING", background);

	//Monitor dpi
	/*
	SetProcessDPIAware(); //ture
	HDC screen = GetDC(NULL);
	double hPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSX);
	double vPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSY);
	ReleaseDC(NULL, screen);
	*/
	//cout << "dpix: "<< hPixelsPerInch <<"dpiy: "<< vPixelsPerInch <<'\n';

	srand(unsigned(std::time(0)));
	for (int i = 0; i < major_count; i++) rand_test_set_one.push_back(i);
	random_shuffle(rand_test_set_one.begin(), rand_test_set_one.end());
	random_shuffle(rand_test_set_one.begin(), rand_test_set_one.end(), random_num);

	srand(unsigned(std::time(0)));
	for (int i = 0; i < minor_count; i++) rand_test_set_two.push_back(i);
	random_shuffle(rand_test_set_two.begin(), rand_test_set_two.end());
	random_shuffle(rand_test_set_two.begin(), rand_test_set_two.end(), random_num);

	VideoCapture cap;    
	cap.open(0);    
	while (cap.isOpened())  
	{
		cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
		cap.read(image);        
		if (!image.empty()) {
			flip(image, image, 1);
			line(image, Point(0, screen_height / 2), Point(screen_width, screen_height / 2), Scalar(0, 0, 255), 2);
			line(image, Point(screen_width /2, 0), Point(screen_width/2, screen_height), Scalar(0, 0, 255), 2);
			if (count == 0) {
				namedWindow("WEBCAM START", CV_WINDOW_NORMAL);
				setWindowProperty("WEBCAM START", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
				imshow("WEBCAM START", image);
			}
			else {
				destroyWindow("WEBCAM START");
				imshow("WEBCAM", image);
			}
			key = waitKey(33); 
			if (key == 115) {
				cout << count << endl;
				if (count == 0) {
					for (i = 0; i < 2; i++) {
						photo_name = "_" + user_pose + "P_" + to_string((start_coordinate[0] + test_set_x - (width_points - 1) / 2)* width_range * 2 / (width_points - 1)) + "H_" + to_string((start_coordinate[1] + test_set_y - (height_points - 1) / 2) * height_range * 2 / (height_points - 1)) + "V_" + to_string(i + 1) + ".png";
						if (i == 0)	waitKey(200);
						cap.read(image);
						flip(image, image, 1);
						imshow("WEBCAM START", image);
						cout << "Save: " << user_name + photo_name << endl;
						imwrite(user_name + "\\" + "Start_" + user_name + photo_name, image, compression_params);
						//imshow(to_string(i), image);
					}
				}
				else {
					for (i = 0; i < 2; i++) {
						photo_name = "_" + user_pose + "P_" + to_string((start_coordinate[0] + test_set_x - (width_points - 1) / 2)* width_range * 2 / (width_points - 1)) + "H_" + to_string((start_coordinate[1] + test_set_y - (height_points - 1) / 2) * height_range * 2 / (height_points - 1)) + "V_" + to_string(i + 1) + ".png";
						if (i == 0)	waitKey(200);
						cap.read(image);
						flip(image, image, 1);
						imshow("WEBCAM", image);
						cout << "Save: " << user_name + photo_name << endl;
						imwrite(user_name + "\\" + user_name + photo_name, image, compression_params);
						//imshow(to_string(i), image);
					}
				}
				if (count < major_count) {
					if ((rand_test_set_one[count] / width_points) == 0) {
						test_set_x = rand_test_set_one[count] % width_points;
						test_set_y = 0;
					}
					else if ((rand_test_set_one[count] / width_points) == 1) {
						test_set_x = rand_test_set_one[count] % width_points;
						test_set_y = (height_points-1)/2;
					}
					else if ((rand_test_set_one[count] / width_points) == 2) {
						test_set_x = rand_test_set_one[count] % width_points;
						test_set_y = height_points-1;
					}
					else if ((rand_test_set_one[count] / width_points) > 2) {
						if (((rand_test_set_one[count] - (width_points*3)) / (height_points-3)) == 0) {
							if (((rand_test_set_one[count] - (width_points * 3)) / ((height_points - 3) / 2)) == 0) {
								test_set_x = 0;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + 1;
							}
							else if (((rand_test_set_one[count] - (width_points * 3)) / ((height_points - 3) / 2)) == 1) {
								test_set_x = 0;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + ((height_points+1)/2);
							}
						}
						else if (((rand_test_set_one[count] - (width_points * 3)) / (height_points - 3)) == 1) {
							if (((rand_test_set_one[count] - ((width_points * 3) + (height_points - 3))) / ((height_points - 3) / 2)) == 0) {
								test_set_x = (width_points-1)/2;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + 1;
							}
							else if (((rand_test_set_one[count] - ((width_points * 3) + (height_points - 3))) / ((height_points - 3) / 2)) == 1) {
								test_set_x = (width_points - 1) / 2;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + ((height_points + 1) / 2);
							}
						}
						else if (((rand_test_set_one[count] - (width_points * 3)) / (height_points - 3)) == 2) {
							if (((rand_test_set_one[count] - ((width_points * 3) + (height_points - 3)*2)) / ((height_points - 3) / 2)) == 0) {
								test_set_x = width_points - 1;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + 1;
							}
							else if (((rand_test_set_one[count] - ((width_points * 3) + (height_points - 3) * 2)) / ((height_points - 3) / 2)) == 1) {
								test_set_x = width_points - 1;
								test_set_y = (rand_test_set_one[count] - (width_points * 3)) % ((height_points - 3) / 2) + ((height_points + 1) / 2);
							}
						}

					}
					//cout << count <<" ," <<rand_test_set_one[count] << '\n';
					show_out_point(background, test_set_x, test_set_y, eye_distance, width_points, width_range, height_points, height_range);
				}
				else if (count<total_count+1){
					if (((rand_test_set_two[(count - major_count)]) / (minor_count/4)) == 0) {
						test_set_x = rand_test_set_two[(count - major_count)] % ((width_points - 3) / 2);
						test_set_y = rand_test_set_two[(count - major_count)] / ((width_points - 3) / 2);
						start_coordinate[0] = 1;
						start_coordinate[1] = 1;
					}
					else if (((rand_test_set_two[(count - major_count)]) / (minor_count / 4)) == 1) {
						test_set_x = (rand_test_set_two[(count - major_count)] - (minor_count / 4)) % ((width_points - 3) / 2);
						test_set_y = (rand_test_set_two[(count - major_count)] - (minor_count / 4)) / ((width_points - 3) / 2);
						start_coordinate[0] = (width_points+1)/2;
						start_coordinate[1] = 1;
					}
					else if (((rand_test_set_two[(count - major_count)]) / (minor_count / 4)) == 2) {
						test_set_x = (rand_test_set_two[(count - major_count)] - (minor_count / 2)) % ((width_points - 3) / 2);
						test_set_y = (rand_test_set_two[(count - major_count)] - (minor_count / 2)) / ((width_points - 3) / 2);
						start_coordinate[0] = 1;
						start_coordinate[1] = (height_points + 1) / 2;
					}
					else if (((rand_test_set_two[(count - major_count)]) / (minor_count / 4)) == 3) {
						test_set_x = (rand_test_set_two[(count - major_count)] - (minor_count * 3 / 4)) % ((width_points - 3) / 2);
						test_set_y = (rand_test_set_two[(count - major_count)] - (minor_count * 3 / 4)) / ((width_points - 3) / 2);
						start_coordinate[0] = (width_points + 1) / 2;
						start_coordinate[1] = (height_points + 1) / 2;
					}
					//cout << count << " ," << rand_test_set_two[(count - major_count)] << '\n';
					show_in_point(background,start_coordinate, test_set_x, test_set_y, eye_distance, width_points, width_range, height_points, height_range);
				}
				key = 0;
				count++;
			}
			else if (key == 27) {
				key = 0;
				break;
			}
			else if (count == total_count+1) {
				break;
			}
		}
	}
	return 0;
}