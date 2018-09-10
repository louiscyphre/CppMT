#ifndef CMT_H

#define CMT_H

#include "cmt_config.h"
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

        vector<Point2f> points_active;
        vector<int> classes_active;
        RotatedRect bb_rot;

        RotatedRect initialMark;

        float theta;

        Mat im_prev;

    } context_t;

    class CMT {
        public:
            CMT() : str_detector(DETECTOR_STR), str_descriptor(DESCRIPTOR_STR),
                    context(nullptr), scale(0.0f), rotation(0.0f),
                    center(0.0f, 0.0f) {};

            context_t* createContext(const Mat &im_gray, const Rect &rect,
              const std::vector<cv::KeyPoint> &points = std::vector<cv::KeyPoint>()) {
                context = new context_t;
                initialize(im_gray, rect, points);
                return context;
            }

            const vector<Point2f> getPoints() const {
                return context->points_active;
            }

            const vector<int> getClasses() const {
                return context->classes_active;
            }

            float getScale() const { return scale; }

            float getRotation() const { return rotation; }

            Point2f getCenter() const { return center; }

            RotatedRect getInitialMark() const { return context->initialMark; }
            RotatedRect getCurrentMark() const { return context->bb_rot; }

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
            void initialize(const Mat &im_gray, const Rect &rect,
                            const std::vector<cv::KeyPoint> &points);
    };

} /* namespace CMT */

#endif /* end of include guard: CMT_H */

