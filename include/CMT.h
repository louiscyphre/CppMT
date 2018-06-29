#ifndef CMT_H

#define CMT_H

#include "common.h"
#include "Consensus.h"
#include "Fusion.h"
#include "Matcher.h"
#include "Tracker.h"

#include <opencv2/features2d/features2d.hpp>

using cv::FeatureDetector;
using cv::DescriptorExtractor;
using cv::Ptr;
using cv::RotatedRect;
using cv::Size2f;

#include <memory>

using std::shared_ptr;

namespace cmt {

    typedef struct {

        Matcher matcher;
        Consensus consensus;

        vector <Point2f> points_active;
        vector<int> classes_active;
        RotatedRect bb_rot;

        Size2f size_initial;
        Point2f center_initial;

        float theta;

        Mat im_prev;

    } context_t;

    class CMT {
        public:
            CMT() : str_detector("FAST"), str_descriptor("ORB"), context
                    (nullptr), scale(0.0f), rotation(0.0f),
                    center(0.0f, 0.0f) {};

            context_t* createNewContext(const Mat &im_gray, const Rect &rect) {
                context = new context_t;
                initialize(im_gray, rect);
                return context;
            }

            context_t* getContext() const { return context; }

            float getScale() const { return scale; }

            float getRotation() const { return rotation; }

            Point2f getCenter() const { return center; }

            Point2f getInitialCenter() const { return context->center_initial; }

            Size2f getInitialRectSize() const { return context->size_initial; }

            void switchContext(context_t* context) {
                this->context = context;
            }

            void processFrame(const Mat im_gray);

            Fusion fusion;
            Tracker tracker;

            string str_detector;
            string str_descriptor;

        private:
            Ptr <FeatureDetector> detector;
            Ptr <DescriptorExtractor> descriptor;

            context_t* context;
            float scale;
            float rotation;
            Point2f center;

            //TODO moved temporarily, fix
            void initialize(const Mat &im_gray, const Rect &rect);
    };

} /* namespace CMT */

#endif /* end of include guard: CMT_H */

