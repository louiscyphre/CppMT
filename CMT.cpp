#include "CMT.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

namespace cmt {

void CMT::initialize(const Mat &im_gray,
                     const std::vector<cv::KeyPoint> &points) {
    FILE_LOG(logDEBUG) << "CMT::initialize() call";

    //Remember initial image
    context->im_prev = im_gray;

    vector<cv::KeyPoint> keypoints;

    if (points.size() == 0) {
        vector<Point2f> coords2f;
        cv::goodFeaturesToTrack(im_gray, coords2f,
                                context->settings.maxPointsToDetect,
                                context->settings.qualityLevelInitial,
                                context->settings.minimalDistance);
        cv::KeyPoint::convert(coords2f, keypoints);
    } else {
        keypoints = points;
    }

    Mat descriptors;
    descriptor = DESCRIPTORS_T::create();
    descriptor->compute(im_gray, keypoints, descriptors);

    //Create initial set of active keypoints
    for (size_t i = 0; i < keypoints.size(); i++) {
        context->points_active.emplace_back(keypoints[i].pt);
        context->classes_active.emplace_back(i);
    }
    //Initialize matcher
    context->matcher.initialize(context->points_active, descriptors,
                                context->classes_active);

    FILE_LOG(logDEBUG) << "CMT::initialize() return";
}

void CMT::processFrame(Mat im_gray) {

    FILE_LOG(logDEBUG) << "CMT::processFrame() call";

    //Track keypoints
    vector<Point2f> points_tracked;
    vector<unsigned char> status;
    tracker.track(context->im_prev, im_gray, context->points_active,
                  points_tracked, status);

    FILE_LOG(logDEBUG) << points_tracked.size() << " tracked points.";

    //keep only successful classes
    vector<int> classes_tracked;
    for (size_t i = 0; i < context->classes_active.size(); i++) {
        if (status[i]) {
            classes_tracked.push_back(context->classes_active[i]);
        }

    }
    //Detect keypoints, compute descriptors
    vector<Point2f> coords2f;
    //The more points is better, later we will remove unnecessary points
    cv::goodFeaturesToTrack(im_gray, coords2f,
                            context->settings.maxPointsToDetect,
                            context->settings.qualityLevelInitial,
                            context->settings.minimalDistance);
    vector<KeyPoint> keypoints;
    cv::KeyPoint::convert(coords2f, keypoints);

    FILE_LOG(logDEBUG) << keypoints.size() << " keypoints found.";

    Mat descriptors;
    descriptor->compute(im_gray, keypoints, descriptors);

    //Match keypoints globally
    vector<Point2f> points_matched_global;
    vector<int> classes_matched_global;
    context->matcher.matchGlobal(keypoints, descriptors, points_matched_global,
                                 classes_matched_global);

    FILE_LOG(logDEBUG) << points_matched_global.size()
                       << " points matched globally.";

    //Fuse tracked and globally matched points
    vector<Point2f> points_fused;
    vector<int> classes_fused;
    fusion.preferFirst(points_tracked, classes_tracked, points_matched_global,
                       classes_matched_global,
                       points_fused, classes_fused);

    context->points_active = points_fused;
    context->classes_active = classes_fused;

    FILE_LOG(logDEBUG) << context->points_active.size()
                       << " final fused points.";

    //Remember current image
    context->im_prev = im_gray;

    FILE_LOG(logDEBUG) << "CMT::processFrame() return";
}

} /* namespace CMT */
