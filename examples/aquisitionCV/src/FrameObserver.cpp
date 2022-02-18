#include <iostream>
#include <iomanip>
#include <time.h>

#include "FrameObserver.h"

namespace AVT {
namespace VmbAPI {

/**
 * @brief Construct a new Frame Observer:: Frame Observer object
 * 
 * @param pCamera  The camera the frame was queued at
 * @param eFrameInfos Indicates how the frame will be displayed
 * @param eColorProcessing Indicates how color processing is applied
 * @param bRGBValue 
 */
FrameObserver::FrameObserver( CameraPtr pCamera, FrameInfos eFrameInfos, ColorProcessing eColorProcessing, bool bRGBValue )
    :   IFrameObserver( pCamera )
    ,   m_eFrameInfos( eFrameInfos )
    ,   m_bRGB( bRGBValue )
    ,   m_eColorProcessing( eColorProcessing )
    ,   proc (new FrameProcessing()) {}

/**
 * @brief get current timestamp
 * 
 * @return double second timestamp
 */
double FrameObserver::GetTime()
{
    double dTime = 0.0;

    struct timespec now;
    clock_gettime( CLOCK_REALTIME, &now );
    dTime = ( (double)now.tv_sec ) + ( (double)now.tv_nsec ) / 1000000000.0;

    return dTime;
}

/**
 * @brief Prints out frame parameters such as w, h, pixel format
 * 
 * @param pFrame The frame to work on
 */
void PrintFrameInfo( const FramePtr &pFrame )
{
    std::cout<<" Size:";
    VmbUint32_t     nWidth = 0;
    VmbErrorType    res;
    res = pFrame->GetWidth(nWidth);
    if( VmbErrorSuccess == res )
    {
        std::cout<<nWidth;
    }
    else
    {
        std::cout<<"?";
    }

    std::cout<<"x";
    VmbUint32_t nHeight = 0;
    res = pFrame->GetHeight(nHeight);
    if( VmbErrorSuccess == res )
    {
        std::cout<< nHeight;
    }
    else
    {
        std::cout<<"?";
    }

    std::cout<<" Format:";
    VmbPixelFormatType ePixelFormat = VmbPixelFormatMono8;
    res = pFrame->GetPixelFormat( ePixelFormat );
    if( VmbErrorSuccess == res )
    {
        std::cout<<"0x"<<std::hex<<ePixelFormat<<std::dec;
    }
    else
    {
        std::cout<<"?";
    }
}

/**
 * @brief Prints out frame status codes as readable status messages
 * 
 * @param eFrameStatus The error code to be converted and printed out
 */
void PrintFrameStatus( VmbFrameStatusType eFrameStatus )
{
    switch( eFrameStatus )
    {
    case VmbFrameStatusComplete:
        std::cout<<"Complete";
        break;

    case VmbFrameStatusIncomplete:
        std::cout<<"Incomplete";
        break;

    case VmbFrameStatusTooSmall:
        std::cout<<"Too small";
        break;

    case VmbFrameStatusInvalid:
        std::cout<<"Invalid";
        break;

    default:
        std::cout<<"unknown frame status";
        break;
    }
}

/**
 * @brief Prints out details of a frame such as: ID, w, h, FPS
 * 
 * @param pFrame The frame to work on
 */
void FrameObserver::ShowFrameInfos( const FramePtr &pFrame ) 
{
    bool                bShowFrameInfos     = false;
    VmbUint64_t         nFrameID            = 0;
    bool                bFrameIDValid       = false;
    VmbFrameStatusType  eFrameStatus        = VmbFrameStatusComplete;
    bool                bFrameStatusValid   = false;
    VmbErrorType        res                 = VmbErrorSuccess;
    double              dFPS                = 0.0;
    bool                bFPSValid           = false;
    VmbUint64_t         nFramesMissing      = 0;
    if( FrameInfos_Show == m_eFrameInfos )
    {
        bShowFrameInfos = true;
    }

    res = pFrame->GetFrameID( nFrameID );
    if( VmbErrorSuccess == res )
    {
        bFrameIDValid = true;

        if( m_FrameID.IsValid() )
        {
            if( nFrameID != ( m_FrameID() + 1 ) )
            {
                nFramesMissing = nFrameID - m_FrameID() - 1;
                if( 1 == nFramesMissing )
                {
                    std::cout<<"1 missing frame detected\n";
                }
                else
                {
                    std::cout<<nFramesMissing<<"missing frames detected\n";
                }
            }
        }

        m_FrameID( nFrameID );
        double dFrameTime = GetTime();
        if(     ( m_FrameTime.IsValid() )
            &&  ( 0 == nFramesMissing ) )
        {
            double dTimeDiff = dFrameTime - m_FrameTime();
            if( dTimeDiff > 0.0 )
            {
                dFPS = 1.0 / dTimeDiff;
                bFPSValid = true;
            }
            else
            {
                bShowFrameInfos = true;
            }
        }

        m_FrameTime( dFrameTime );
    }
    else
    {
        bShowFrameInfos = true;
        m_FrameID.Invalidate();
        m_FrameTime.Invalidate();
    }

    res = pFrame->GetReceiveStatus( eFrameStatus );
    if( VmbErrorSuccess == res )
    {
        bFrameStatusValid = true;

        if( VmbFrameStatusComplete != eFrameStatus )
        {
            bShowFrameInfos = true;
        }
    }
    else
    {
        bShowFrameInfos = true;
    }
    if( bShowFrameInfos )
    {
        std::cout<<"Frame ID:";
        if( bFrameIDValid )
        {
            std::cout<<nFrameID;
        }
        else
        {
            std::cout<<"?";
        }

        std::cout<<" Status:";
        if( bFrameStatusValid )
        {
            PrintFrameStatus( eFrameStatus);
        }
        else
        {
            std::cout<<"?";
        }
        PrintFrameInfo( pFrame );
        
        std::cout<<" FPS:";
        if( bFPSValid )
        {
            std::streamsize s = std::cout.precision();
            std::cout<<std::fixed<<std::setprecision(2)<<dFPS<<std::setprecision(s);
        }
        else
        {
            std::cout<<"?";
        }

        std::cout<<"\n";
    }
    else
    {
        std::cout<<".";
    }
}

/**
 * @brief This is our callback routine that will be executed on every received frame.
 * 
 * @param pFrame The frame returned from the API
 */
void FrameObserver::FrameReceived( const FramePtr pFrame )
{
    if(!SP_ISNULL( pFrame ) )
    {
        if( FrameInfos_Off != m_eFrameInfos )
        {
            ShowFrameInfos( pFrame);
        }

        VmbFrameStatusType status;
        VmbErrorType Result;

        Result = SP_ACCESS( pFrame)->GetReceiveStatus( status);

        if( VmbErrorSuccess == Result && VmbFrameStatusComplete == status)
        {            
            /**
             * @brief Funzione per la conversione del buffer raw in oggetto CV
             * 
             */
            proc->ProcessImage(pFrame);
        }
        else
        {
            std::cout<<"frame incomplete\n";
        }
    }
    else
    {
        std::cout <<" frame pointer NULL\n";
    }

    m_pCamera->QueueFrame( pFrame );
}
}} // namespace AVT::VmbAPI
