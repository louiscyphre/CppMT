#ifndef MATCHER_H

#define MATCHER_H

#include "common.h"

#include "opencv2/features2d/features2d.hpp"

using cv::KeyPoint;
using cv::Ptr;
using cv::DescriptorMatcher;

namespace cmt {

class Matcher
{
public:
        Matcher() : thr_dist(0.25), thr_ratio(0.8), thr_cutoff(20) {};
        //Matcher() : thr_dist(0.01), thr_ratio(0.75), thr_cutoff(15) {};
    void initialize(const vector<Point2f> & points, const Mat
        &descriptors, const vector<int> &classes);
    void matchGlobal(const vector<KeyPoint> & keypoints, const Mat descriptors,
            vector<Point2f> & points_matched, vector<int> & classes_matched);

private:
    vector<Point2f> points;
    Mat database;
    vector<int> classes;
    int desc_length;
    Ptr<DescriptorMatcher> bfmatcher;
    float thr_dist;
    float thr_ratio;
    float thr_cutoff;
};

} /* namespace CMT */

#endif /* end of include guard: MATCHER_H */
