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

    enum class Settings {
            maxPointsToDetect,
            minimalDistance,
            qualityLevelInitial,
            qualityLevelDetection
    };

    typedef struct {
        int maxPointsToDetect = MAX_POINTS;
        double minimalDistance = MIN_DISTANCE;
        double qualityLevelInitial = POINTS_QUALITY_LEVEL_INIT;
        double qualityLevelDetection = POINTS_QUALITY_LEVEL_DETECT;
    } settings_t;

    settings_t settings;

} context_t;

class CMT {
    public:
        CMT() : str_detector(DETECTOR_STR), str_descriptor(DESCRIPTOR_STR),
                context(nullptr) {};

        context_t* createContext(const Mat &im_gray,
                                 const std::vector<cv::KeyPoint> &points =
                                 std::vector<cv::KeyPoint>(),
                                 const cv::Mat &descriptors = cv::Mat()) {
            context = new context_t;
            initialize(im_gray, points, descriptors);
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

        template<typename T>
        void adjust(const T &value,
                    const context_t::Settings &setting) {
            //TODO without switch
            switch (setting) {
                case context_t::Settings::maxPointsToDetect:
                    context->settings.maxPointsToDetect = value;
                case context_t::Settings::minimalDistance:
                    context->settings.minimalDistance = value;
                case context_t::Settings::qualityLevelInitial:
                    context->settings.qualityLevelInitial = value;
                case context_t::Settings::qualityLevelDetection:
                    context->settings.qualityLevelDetection = value;
            }
        }

    private:
        Ptr<FeatureDetector> detector;
        Ptr<DescriptorExtractor> descriptor;

        Fusion fusion;
        Tracker tracker;

        string str_detector;
        string str_descriptor;

        context_t* context;

        //TODO moved temporarily, fix
        void initialize(const Mat &im_gray,
                        const std::vector<cv::KeyPoint> &points,
                        const cv::Mat &descriptors);
};

} /* namespace CMT */

#endif /* end of include guard: CMT_H */

