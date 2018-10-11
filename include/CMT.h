#ifndef CMT_H

#define CMT_H

#include "cmt_config.h"
#include "common.h"
#include "Fusion.h"
#include "Matcher.h"
#include "Tracker.h"

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

using cv::FeatureDetector;
using cv::DescriptorExtractor;
using cv::Ptr;
using cv::Size2f;

namespace cmt {

    typedef struct {

        Matcher matcher;

        vector<Point2f> points_active;
        vector<int> classes_active;

        Mat im_prev;

    } context_t;

    class CMT {
        public:
            CMT() : str_detector(DETECTOR_STR), str_descriptor(DESCRIPTOR_STR),
                    context(nullptr){};

            context_t* createContext(const Mat &im_gray, //const Rect &rect,
              const std::vector<cv::KeyPoint> &points = std::vector<cv::KeyPoint>()) {
                context = new context_t;
                initialize(im_gray, points);
                return context;
            }

            const vector<Point2f> getPoints() const {

                std::vector<Point2f> points;
                points = context->points_active;

                return points;
            }

            const vector<int> getClasses() const {
                return context->classes_active;
            }

            void switchContext(context_t* context) {
                this->context = context;
            }

            void processFrame(const Mat im_gray);

        private:
            Ptr <FeatureDetector> detector;
            Ptr <DescriptorExtractor> descriptor;

            Fusion fusion;
            Tracker tracker;

            string str_detector;
            string str_descriptor;

            context_t* context;

            //TODO moved temporarily, fix
            void initialize(const Mat &im_gray,
                            const std::vector<cv::KeyPoint> &points);
    };

} /* namespace CMT */

#endif /* end of include guard: CMT_H */

