#ifndef FRAME_PROCESSING
#define FRAME_PROCESSING

#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaImageTransform/Include/VmbTransform.h"
#include <opencv2/opencv.hpp>

namespace AVT {
namespace VmbAPI {

class FrameProcessing {
    public: 
        FrameProcessing();
        
        void        ProcessImage(const FramePtr);
        void        Show();

        VmbImage    GetImage();
        cv::Mat     GetCVImage();

    private: 
        VmbImage    sourceImage;
        cv::Mat     cvImage;
};

}}

#endif