#include "FrameProcessing.h"

namespace AVT {
namespace VmbAPI {

FrameProcessing::FrameProcessing() {}

void FrameProcessing::ProcessImage(const FramePtr pFrame)
{
    VmbUint32_t Height = 0;
    VmbUint32_t Width  = 0;
    VmbPixelFormatType pixelF;

    pFrame->GetWidth(Width);
    pFrame->GetHeight(Height);
    pFrame->GetPixelFormat(pixelF);

    VmbUchar_t *pBuffer;
    VmbErrorType err = SP_ACCESS( pFrame )->GetImage( pBuffer );

    VmbSetImageInfoFromPixelFormat( pixelF, Width, Height, & this->sourceImage );
    this->sourceImage.Data = pBuffer;
    this->sourceImage.Size = sizeof( this->sourceImage );

    cv::Mat imageLocal(cv::Size(Width, Height), CV_8UC3, (void*) this->sourceImage.Data);
}

void FrameProcessing::Show()
{
    cv::imshow("Streaming Vimba", this->cvImage);
    cv::waitKey(1);
}

}}