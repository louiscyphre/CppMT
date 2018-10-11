#include "Matcher.h"

using cv::DMatch;

namespace cmt {

void Matcher::initialize(const vector<Point2f> & points, const Mat
&descriptors, const vector<int> &classes)
{
    FILE_LOG(logDEBUG) << "Matcher::initialize() call";

    this->points = points;

    database = descriptors;

    //Extract descriptor length from features
    desc_length = database.cols*8;

    this->classes = classes;

    //Create descriptor matcher
    bfmatcher = DescriptorMatcher::create("BruteForce-Hamming");

    FILE_LOG(logDEBUG) << "Matcher::initialize() return";
}

void Matcher::matchGlobal(const vector<KeyPoint> & keypoints, const Mat descriptors,
        vector<Point2f> & points_matched, vector<int> & classes_matched)
{
    FILE_LOG(logDEBUG) << "Matcher::matchGlobal() call";

    if (keypoints.size() == 0)
    {
        FILE_LOG(logDEBUG) << "Matcher::matchGlobal() return";
        return;
    }

    vector<vector<DMatch> > matches;
    bfmatcher->knnMatch(descriptors, database, matches, 2);

    for (size_t i = 0; i < matches.size(); i++)
    {
        vector<DMatch> m = matches[i];

        float distance1 = m[0].distance / desc_length;
        float distance2 = m[1].distance / desc_length;
        int matched_class = classes[m[0].trainIdx];

        if (matched_class == -1) continue;
        if (distance1 > thr_dist) continue;
        if (distance1/distance2 > thr_ratio) continue;

        points_matched.push_back(keypoints[i].pt);
        classes_matched.push_back(matched_class);
    }

    FILE_LOG(logDEBUG) << "Matcher::matchGlobal() return";
}


} /* namespace CMT */
