#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <raspicam/raspicam_cv.h>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <time.h>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <cmath>
#include <algorithm>
#include <regex>

#include "opencv2/calib3d.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <list>
#include <vector>



using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;


//using namespace raspicam;

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

vector<DMatch> matches;


vector<Point2f> old_frame_pts, new_frame_pts;
vector<KeyPoint> old_frame_kpts, new_frame_kpts;
vector<unsigned char> match_mask;
Ptr<BriefDescriptorExtractor> brief;
Ptr<FastFeatureDetector> detector;



Mat new_frame, old_frame, gray, H, old_H, old_frame_desc, new_frame_desc;

/*** HOMOGRAPHY ****/

namespace
{
    void drawMatchesRelative(const vector<KeyPoint>& train, const vector<KeyPoint>& query, std::vector<cv::DMatch>& matches, Mat& img, const vector<unsigned char>& mask = vector<unsigned char>())
    {
        for (int i = 0; i < (int)matches.size(); i++)
        {
            if (mask.empty() || mask[i])
            {
                Point2f pt_new = query[matches[i].queryIdx].pt;
                Point2f pt_old = train[matches[i].trainIdx].pt;

                cv::line(img, pt_new, pt_old, Scalar(0, 0, 255), 1);
                cv::circle(img, pt_new, 2, Scalar(255, 0, 125), 1);

            }
        }
    }

    //Takes a descriptor and turns it into an xy point
    void keypoints2points(const vector<KeyPoint>& in, vector<Point2f>& out)
    {
        out.clear();
        out.reserve(in.size());
        for (size_t i = 0; i < in.size(); ++i)
        {
            out.push_back(in[i].pt);
        }
    }

    //Takes an xy point and appends that to a keypoint structure
    void points2keypoints(const vector<Point2f>& in, vector<KeyPoint>& out)
    {
        out.clear();
        out.reserve(in.size());
        for (size_t i = 0; i < in.size(); ++i)
        {
            out.push_back(KeyPoint(in[i], 1));
        }
    }

    //Uses computed homography H to warp original input points to new planar position
    void warpKeypoints(const Mat& H, const vector<KeyPoint>& in, vector<KeyPoint>& out)
    {
        vector<Point2f> pts;
        keypoints2points(in, pts);
        vector<Point2f> pts_w(pts.size());
        Mat m_pts_w(pts_w);
        perspectiveTransform(Mat(pts), m_pts_w, H);
        points2keypoints(pts_w, out);
    }

    //Converts matching indices to xy points
    void matches2points(const vector<KeyPoint>& train, const vector<KeyPoint>& query,
        const std::vector<cv::DMatch>& matches, std::vector<cv::Point2f>& pts_train,
        std::vector<Point2f>& pts_query)
    {

        pts_train.clear();
        pts_query.clear();
        pts_train.reserve(matches.size());
        pts_query.reserve(matches.size());

        size_t i = 0;

        for (; i < matches.size(); i++)
        {

            const DMatch & dmatch = matches[i];

            pts_query.push_back(query[dmatch.queryIdx].pt);
            pts_train.push_back(train[dmatch.trainIdx].pt);

        }

    }

    void resetH(Mat&H)
    {
        H = Mat::eye(3, 3, CV_32FC1);
    }
}

/*******/




vector<string> leer_imagenes(const string& ruta_imagenes)
{
    vector<string> imagenes;
    dirent* img;
    DIR* directorio;
    directorio = opendir(ruta_imagenes.c_str());
    if (directorio)
    {
        img = readdir(directorio);
        while (img != nullptr)
      {
      imagenes.push_back(string(img->d_name));
      img = readdir(directorio);
      }
        closedir(directorio);
        //ordenamos el vector con los nombres
        //sort( imagenes.begin(), imagenes.end() );
    }
    return imagenes;
}


void process(Ptr<BriefDescriptorExtractor> brief, BFMatcher descriptor_match){
    if(!new_frame_kpts.empty()){
        vector<KeyPoint> kpts_t;
        warpKeypoints(old_H.inv(), old_frame_kpts, kpts_t);

        descriptor_match.match(old_frame_desc, new_frame_desc, matches, Mat());
        drawKeypoints(old_frame, kpts_t, old_frame, Scalar(255,0,0), 1);

        matches2points(new_frame_kpts, old_frame_kpts, matches, new_frame_pts, old_frame_pts);

        if(matches.size() >= 5){
            Mat H = findHomography(new_frame_pts, old_frame_pts, CV_RANSAC, 4, match_mask);
            if (countNonZero(Mat(match_mask)) > 15)
            {
                old_H = H;
            }
            else
                resetH(old_H);
            drawMatchesRelative(new_frame_kpts, old_frame_kpts, matches, old_frame, match_mask);
        }else{
            resetH(old_H);
        }

    }
}

int main(int argc, char *argv[])
{

   // string img1 = argv[1];
   // string img2 = argv[2];

    vector<string> imgs_name = leer_imagenes("images_hdr/");
    vector<string> imgs_name_sort(imgs_name.size()-2);

    for(int i=2; i<imgs_name.size(); i++){
        regex rgx("^[0-9]+");
        smatch match;
        string s = imgs_name[i];


        if(regex_search(s, match, rgx)){
            string number(match[0]);
            int n = atoi(number.c_str());

            imgs_name_sort[n] = s;
        }

    }


   // namedWindow("IMG", CV_WINDOW_NORMAL);
   // namedWindow("IMG2", CV_WINDOW_NORMAL);
   // namedWindow("diff", CV_WINDOW_NORMAL);
   // namedWindow("original", CV_WINDOW_NORMAL);
    /*if(argv[1] != ""){
        string ruta_img(argv[1]);

        Mat img = imread(ruta_img.c_str());
        imshow("IMG", img);
    }
    else{
        return -1;
    }
*/

    vector<Mat> images;
    for(int i=0; i<200; i++){
        Mat im = imread("images_hdr/"+imgs_name_sort[i]);
        images.push_back(im);
    }

    //DIFERENCIA ENTRE LAS IMAGENES
    /*Mat img_diff;
    absdiff(images[100],images[101],img_diff);

    //ESCALA DE GRISES
    Mat gray;
    cvtColor(img_diff, gray, CV_RGB2GRAY);

    int ratio=3;
    int umbral = 50;
    //int kernel = 3;

    //Eliminar ruido
    Mat ruido;
    Mat kernel = Mat::eye(3,3,CV_32F)/(float)(9);
    morphologyEx(gray, ruido, MORPH_OPEN, kernel);


    //Edges
    Mat edges;
    //Canny(gray, edges, umbral, umbral*ratio, kernel);
    threshold(ruido, edges, umbral, 255, 0);
    //CONTORNOS
    Mat contours;
    vector<Vec4i> hierarchy;
    //findContours(edges, contours, hierarchy,  RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );




    imshow("diff", img_diff);
    imshow("IMG", edges);
*/

    brief = BriefDescriptorExtractor::create(32);
    detector = FastFeatureDetector::create(10, true);

    BFMatcher descriptor_match(brief->defaultNorm());

    old_H = Mat::eye(3,3, CV_32FC1);



    //PRIMERA IMAGEN QUE PROCESAMOS

    //Imagen a procesar anterior
   /* new_frame = images[1];


    //Paamos a escala de grises
    cvtColor(new_frame, gray, CV_RGB2GRAY);
    //Detectamos los puntos de interes
    detector->detect(gray, new_frame_kpts);
    //Descriptores en cada punto de interes
    brief->compute(gray, new_frame_kpts, new_frame_desc);


    if(old_frame_kpts.empty()){
        old_H = Mat::eye(3,3, CV_32FC1);
        Mat out;
        drawKeypoints(new_frame, new_frame_kpts, out);
        new_frame = out;
    }



    namedWindow(to_string(1), CV_WINDOW_NORMAL);
    imshow("1", new_frame);


    old_frame_kpts = new_frame_kpts;
    new_frame_desc.copyTo(old_frame_desc);
*/


    for(int i=100; i<=130;){
        new_frame = images[i];

        cvtColor(new_frame, gray, CV_RGB2GRAY);

        detector->detect(gray, new_frame_kpts);
        brief->compute(gray, new_frame_kpts, new_frame_desc);

        if(!old_frame_kpts.empty()){
            vector<KeyPoint> kpts;
            warpKeypoints(old_H.inv(), new_frame_kpts, kpts);

            descriptor_match.match(new_frame_desc, old_frame_desc, matches, Mat());
            drawKeypoints(new_frame, kpts, new_frame, Scalar(255,0,0), 1);

            matches2points(old_frame_kpts, new_frame_kpts, matches, old_frame_pts, new_frame_pts);

            cout << matches.size() << endl;

            if(matches.size() > 100){
                Mat H = findHomography(old_frame_pts, new_frame_pts, CV_RANSAC, 4, match_mask);

                cout << countNonZero(Mat(match_mask)) << endl;
                if(countNonZero(Mat(match_mask)) > 150)
                    old_H = H;
                else
                    resetH(old_H);


                drawMatchesRelative(old_frame_kpts, new_frame_kpts, matches, new_frame, match_mask);

            }
            else
                resetH(old_H);
        }else{
            old_H = Mat::eye(3,3, CV_32FC1);
            Mat out;
            drawKeypoints(new_frame, new_frame_kpts, out);
            new_frame = out;
        }


        namedWindow(to_string(i), CV_WINDOW_NORMAL);
        imshow(to_string(i), new_frame);


        if(i%2 == 0){
            old_frame_kpts = new_frame_kpts;
            new_frame_desc.copyTo(old_frame_desc);
            resetH(old_H);
        }


        i = i + 5;

    }


    waitKey(0);
    return 0;
}
