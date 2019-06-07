//
//  main.cpp
//  assignment1
//
//  Created by Mavilla Jyothi on 2/09/19.
//  Copyright © 2019 Mavilla Jyothi. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define LOOPNUM 2000
using namespace cv;

//define variables

cv::Mat girl[13];
cv::Mat dinosaur[13];
cv::Mat girl_mask[13];
cv::Mat dinosaur_mask[13];
cv::Mat bg_scene; // background image
cv::Mat screen_img;
cv::Point girl_top_left; // define the position of girl
cv::Point dinosaur_top_left; // define the position of dinosaur
int scene_moving_speed = 20;

//initialise variable for position

void init()
{
    girl_top_left.x = 600;
    girl_top_left.y = 350;
}



// load images

void images()
{
    for (int i = 0; i < 13; i++)
    {
        char file_name[200];
        
        // load girl texture image and mask
        
        sprintf(file_name, "girl%02d.jpg", i);
        girl[i] = cv::imread(file_name);
        sprintf(file_name, "girl_mask_%02d.bmp", i);
        girl_mask[i] = cv::imread(file_name, 0);
        
       // load dinosaur texture image and mask
        
        sprintf(file_name, "dinosaur%02d.jpg", i);
        dinosaur[i] = cv::imread(file_name);
        sprintf(file_name, "dinosaur_mask_%02d.bmp", i);
        dinosaur_mask[i] = cv::imread(file_name, 0);
        
    }
    
   // load background scene
    
    bg_scene = cv::imread("bg_scene.jpg");
    
    // create the screen
    
    screen_img = cv::Mat::zeros(bg_scene.rows, bg_scene.cols, CV_8UC3);
}
// draw girl img on screen

void girlImg(int idx)
{
    girl[idx].copyTo(screen_img(cv::Range(girl_top_left.y, girl_top_left.y + girl[idx].rows), cv::Range(girl_top_left.x, girl_top_left.x + girl[idx].cols)), girl_mask[idx]);
}
// draw dinosaur img on screen

void dinosaurImg(int idx)
{
    dinosaur_top_left.y = girl_top_left.y + girl[idx].rows - dinosaur[idx].rows;
    dinosaur_top_left.x = girl_top_left.x - dinosaur[idx].cols;
    dinosaur[idx].copyTo(screen_img(cv::Range(dinosaur_top_left.y, dinosaur_top_left.y + dinosaur[idx].rows), cv::Range(dinosaur_top_left.x, dinosaur_top_left.x + dinosaur[idx].cols)), dinosaur_mask[idx]);
}

// moving background

void backgroundImg(int idx_bg)
{
    // calculate the left and right boundaries
    int left = (idx_bg * scene_moving_speed % bg_scene.cols);
    int right = left + bg_scene.cols;
    if (right <= bg_scene.cols) {
        bg_scene(cv::Range(0, bg_scene.rows), cv::Range(left, right)).copyTo(screen_img);
    }
    else {
        int remainder = right - bg_scene.cols;
        //copy part A
        bg_scene(cv::Range(0, bg_scene.rows), cv::Range(left, bg_scene.cols)).copyTo(screen_img(cv::Range(0, bg_scene.rows), cv::Range(0, bg_scene.cols - left)));
        //copy the remaining part B
        bg_scene(cv::Range(0, bg_scene.rows), cv::Range(0, remainder)).copyTo(screen_img(cv::Range(0, bg_scene.rows), cv::Range(bg_scene.cols - left, screen_img.cols)));
    }
}

void processImg(int idx, int idx_bg)
{
    // refresh image in the starting
    screen_img = cv::Mat::zeros(bg_scene.rows, bg_scene.cols, CV_8UC3);
  // draw background
    backgroundImg(idx_bg);
   // draw girl
    girlImg(idx);
   // draw dinosaur
    dinosaurImg(idx);
}
int main(int argc, const char * argv[])
{
    // initialise variables
    init();
   // load images
    images();
    
    //index of girl and dinosaur frame
    int idx = 0;
    //index for background image
    int idx_bg = 0;
    for (;;)
    {
        processImg(idx, idx_bg);
        cv::imshow("forest", screen_img);
        char c = waitKey(1);
        if (c == 27)
            break;
       //keyboard operations
        // to move upwards
        if (c == 'w' || c == 'W')
        {
            //check whether out of range
            if ((girl_top_left.y > 10) && (girl_top_left.y + girl[0].rows - dinosaur[0].rows) > 10)
                girl_top_left.y -= 10;
            
        }
         // to move leftwards
        if (c == 'a' || c == 'A')
        {
            if ((girl_top_left.x - dinosaur[0].cols) > 10)
                girl_top_left.x -= 10;
        }
        // to move rightwards
        if (c == 'd' || c == 'D')
        {
            if ((girl_top_left.x + girl[0].cols) < (screen_img.cols - 10))
                girl_top_left.x += 10;
        }
        // to move downwards
        if (c == 's' || c == 'S')
        {
            //check whether out of range
            if ((girl_top_left.y + girl[0].rows) < (screen_img.cols - 10))
                girl_top_left.y += 10;
        }
        // update index numbers
        if (idx < 12)
            idx++;
        else
            idx = 0;
        if (idx_bg < LOOPNUM)
            idx_bg++;
        else
            idx_bg = 0;
    }
    
    return 0;
}
