//this is a lab in my computer vision course with some additional comments
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int main(int argc, char** argv )
{
	int solid;
	cout << "What platonic solid? (0 for tetrahedron, 1 for cube, 2 for octahedron)" << "\n";
	cin >> solid;
	//store images of rotations per degree for 360 degree rotations about the z axis
    vector<Mat> frames;


	//Matrices carrying vertices of solids
	Mat tetra = (Mat_<double>(3,4) << 300 * 1/sqrt(3), 0, -300 * sqrt(3)/6, - 300 * sqrt(3)/6, 0, 0, 300 * 1.0/2, -300 * 1.0/2, 0, 300 * 2/sqrt(6), 0, 0);
	Mat cube = (Mat_<double>(3,8) << 300, 300, -300, -300, 300, 300, -300, -300, 300, 300, 300, 300, -300, -300, -300, -300, 300, -300, 300, -300, 300, -300,300, -300);
	Mat octa = (Mat_<double> (3,6) << 300, -300, 0, 0, 0, 0, 0, 0, 300, -300, 0, 0, 0, 0, 0, 0, 300, -300);
	double theta;
	double theta1;
	double theta2;
	for(theta2 = 0; theta2 < 2* M_PI; theta2 += M_PI/180){
		Mat image(800, 800, CV_8UC3, Scalar(0,0,0));
	  Mat rotz = (Mat_<double>(3,3) << cos(theta2), -sin(theta2), 0, sin(theta2), cos(theta2), 0, 0, 0, 1);

		int i = 1;
		int j = 2;
		//matrix representing new vertices determined through matrix mulitiplcation
		Mat points;
		if(solid == 0) points = rotz * tetra;
		if(solid == 1) points = rotz * cube;
		if(solid == 2) points = rotz * octa;

		//matrix reprsenting projection new positions of vertexes onto a 2d plane to eventually become a frame
		Mat proj;
		if(solid == 0)
			proj = (Mat_<double>(2,4) <<(-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(1, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(1, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(1, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(1, 3), (-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(2, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(2, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(2, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(2, 3));
 		if(solid == 1)
      proj = (Mat_<double>(2,8) <<(-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(1, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(1, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(1, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(1, 3), (-900/(points.at<double>(0, 4) - 1000)) * points.at<double>(1, 4), (-900/(points.at<double>(0, 5) - 1000)) * points.at<double>(1, 5), (-900/(points.at<double>(0, 6) - 1000)) * points.at<double>(1, 6), (-900/(points.at<double>(0, 7) - 1000)) * points.at<double>(1, 7),(-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(2, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(2, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(2, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(2, 3), (-900/(points.at<double>(0, 4) - 1000)) * points.at<double>(2, 4), (-900/(points.at<double>(0, 5) - 1000)) * points.at<double>(2, 5), (-900/(points.at<double>(0, 6) - 1000)) * points.at<double>(2, 6), (-900/(points.at<double>(0, 7) - 1000)) * points.at<double>(2, 7));
 		if(solid == 2)
      proj = (Mat_<double>(2,6) <<(-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(1, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(1, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(1, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(1, 3), (-900/(points.at<double>(0, 4) - 1000)) * points.at<double>(1, 4), (-900/(points.at<double>(0, 5) - 1000)) * points.at<double>(1, 5), (-900/(points.at<double>(0, 0) - 1000)) * points.at<double>(2, 0), (-900/(points.at<double>(0, 1) - 1000)) * points.at<double>(2, 1), (-900/(points.at<double>(0, 2) - 1000)) * points.at<double>(2, 2), (-900/(points.at<double>(0, 3) - 1000)) * points.at<double>(2, 3), (-900/(points.at<double>(0, 4) - 1000)) * points.at<double>(2, 4), (-900/(points.at<double>(0, 5) - 1000)) * points.at<double>(2, 5));

  	//convert new set of vertices to correct location for a 800 by 800 image
		for(int r = 0; r < proj.rows; r++)
    	for(int c = 0; c < proj.cols; c++)
        proj.at<double>(r, c) += 400;

  	//draw circles for vertices on the next frame
		for( size_t x = 0; x < proj.cols; x++){

      Point cen = Point(round(proj.at<double>(0, x)),round( proj.at<double>(1, x)));
      circle(image, cen, 5, Scalar(255, 255, 255), 3, LINE_AA);

		}
		//draw lines for edges on the next frame
		for(int x = 0; x < proj.cols; x++){
			for(int y = x + 1 ; y < proj.cols; y++){
				Point s =  Point(round(proj.at<double>(0, x)),round( proj.at<double>(1, x)));
				Point e =  Point(round(proj.at<double>(0, y)),round( proj.at<double>(1, y)));
				line(image, s, e, Scalar(255, 255, 255),1, 8, 0);
		}

		}
	frames.push_back(image);
	}
	//create a video of the frames stored
	Size s = frames[0].size();
	VideoWriter rotation("./movie.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, s);



	for(int f = 0; f < frames.size(); f++)
		rotation.write(frames[f]);

  waitKey(0);

  return 0;
}
//
//end of file
//
