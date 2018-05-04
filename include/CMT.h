#ifndef CMT_H

#define CMT_H

#include "common.h"
#include "Consensus.h"
#include "Fusion.h"
#include "Matcher.h"
#include "Tracker.h"

#include <opencv2/features2d/features2d.hpp>
#include <memory>

using cv::FeatureDetector;
using cv::DescriptorExtractor;
using cv::Ptr;
using cv::RotatedRect;
using cv::Size2f;

namespace cmt {

    typedef struct {

        Matcher matcher;
        Consensus consensus;

        vector<Point2f> points_active;
        vector<int> classes_active;
        RotatedRect bb_rot;

        Size2f size_initial;

        float theta;

        Mat im_prev;

    } context_t;

    class CMT {
    public:
        CMT() : str_detector("FAST"), str_descriptor("BRISK") {};

        void initialize(const Mat im_gray, const Rect rect);

        void switchContext(context_t *context) {
            this->context = context;
        }

        context_t* getContext() const { return context; }

        void processFrame(const Mat im_gray);

        Fusion fusion;
        Tracker tracker;

        string str_detector;
        string str_descriptor;

    private:
        Ptr<FeatureDetector> detector;
        Ptr<DescriptorExtractor> descriptor;

        context_t *context;
    };

} /* namespace CMT */

#endif /* end of include guard: CMT_H */
