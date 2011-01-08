/* 
 *  Copyright (c) 2011  Chen Feng (cforrest (at) umich.edu)
 *    and the University of Michigan
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

/* MultiViewSparseRec.cpp */

#include "MultiViewSparseRec.h"
#include "Log.h"

MVSR::MVSR(void)
{
	detector = new cv::SurfFeatureDetector();
	TagI("detector = SURF\n");

	descriptor = new cv::SurfDescriptorExtractor();
	TagI("descriptor = SURF\n");

	matcher = new cv::FlannBasedMatcher();
	TagI("matcher = Flann\n");
}

MVSR::~MVSR(void)
{
	delete detector;
	detector = 0;
	delete descriptor;
	descriptor = 0;
	delete matcher;
	matcher = 0;
}

bool MVSR::run(vector<string> imgnamelist)
{
	if( !detector || !descriptor || !matcher ) {
		TagE("No valid detector or descriptor or matcher\n");
		return false;
	}

	pictures.clear();
	clouds.clear();
	matcher->clear();

	if(!loadimage(imgnamelist)) return false;
	if(!detect())				return false;
}

bool MVSR::loadimage(vector<string> imgnamelist)
{
	int nimg = (int)imgnamelist.size();
	pictures.resize(nimg);
	int im=0,in=0;
	for(; in<nimg; ++in) {
		string& name = imgnamelist[in];
		MVSRpicture& pic = pictures[im];
		pic.img = imread(name);
		if(pic.img.empty()) {
			TagW("can't open %s\n", name.c_str());
			continue;
		} else {
			TagI("opened %s\n", name.c_str());
		}
		cvtColor(pic.img, pic.grey, CV_RGB2GRAY);
		++im;
	}
	pictures.resize(im);
	return im==0?false:true;
}

bool MVSR::detect()
{
	double tt = (double)getTickCount();
	for(int i=0; i<(int)pictures.size(); ++i) {
		MVSRpicture& pic = pictures[i];
		detector->detect(pic.grey, pic.key);
		descriptor->compute(pic.grey, pic.key, pic.des);
	}
	tt = (double)getTickCount() - tt;
	TagI("detect+describe time = %lf s\n", tt/getTickFrequency());
	return true;
}

bool MVSR::match()
{
	double tt = (double)getTickCount();
	vector<Mat> desMat;
	desMat.reserve(pictures.size());
	for(int i=0; i<(int)pictures.size(); ++i) {
		MVSRpicture& pic = pictures[i];
		desMat.push_back(pic.des);
	}
	matcher->add(desMat);
	for(int i=0; i<(int)pictures.size(); ++i) {
		MVSRpicture& pic = pictures[i];
		vector<DMatch> matches;
		matcher->match(pic.des, matches);
		//parse matches
		for(int j=0; j<(int)matches.size(); ++j) {
			const DMatch& m = matches[j];
			if(m.imgIdx==i) continue; //match to self, ignore
			TODO
		}
	}
	tt = (double)getTickCount() - tt;
	TagI("match time = %lf s\n", tt/getTickFrequency());
	return true;
}