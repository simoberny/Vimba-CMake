#include <sstream>
#include <iostream>

#include "ApiController.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

namespace AVT {
namespace VmbAPI {

#define NUM_FRAMES 3

ApiController::ApiController()
    // Get a reference to the Vimba singleton
    : m_system ( VimbaSystem::GetInstance() )
{}

ApiController::~ApiController()
{
}

//
// Translates Vimba error codes to readable error messages
//
// Parameters:
//  [in]    eErr        The error code to be converted to string
//
// Returns:
//  A descriptive string representation of the error code
//
std::string ApiController::ErrorCodeToMessage( VmbErrorType eErr ) const
{
    return AVT::VmbAPI::ErrorCodeToMessage( eErr );
}

//
// Starts the Vimba API and loads all transport layers
//
// Returns:
//  An API status code
//
VmbErrorType ApiController::StartUp()
{
    return m_system.Startup();
}

//
// Shuts down the API
//
void ApiController::ShutDown()
{
    // Release Vimba
    m_system.Shutdown();
}

//
// Opens the given camera
// Sets the maximum possible Ethernet packet size
// Adjusts the image format
// Sets up the observer that will be notified on every incoming frame
// Calls the API convenience function to start image acquisition
// Closes the camera in case of failure
//
// Parameters:
//  [in]    Config      A configuration struct including the camera ID and other settings
//
// Returns:
//  An API status code
//
VmbErrorType ApiController::StartContinuousImageAcquisition( const ProgramConfig& Config )
{
    // Open the desired camera by its ID
    VmbErrorType res = m_system.OpenCameraByID( Config.getCameraID().c_str(), VmbAccessModeFull, m_pCamera );
    if ( VmbErrorSuccess == res )
    {
        if ( VmbErrorSuccess == res )
        {
            /**
             * @brief La funzione estrae altezza e larghezza dell'immagine direttamente dalle informazioni della camera
             * perchè verranno utilizzate per l'allocamento del buffer che ospiterà le vere immagini
             */
            res = PrepareCamera();
            if ( VmbErrorSuccess == res )
            {
                // Create a frame observer for this camera (This will be wrapped in a shared_ptr so we don't delete it)
                m_pFrameObserver = new FrameObserver( m_pCamera, Config.getFrameInfos(), Config.getColorProcessing(), Config.getRGBValue() );
                // Start streaming
                res = m_pCamera->StartContinuousImageAcquisition( NUM_FRAMES, IFrameObserverPtr( m_pFrameObserver ), Config.getAllocAndAnnounce() ? FrameAllocation_AllocAndAnnounceFrame : FrameAllocation_AnnounceFrame );
            }
        }

        if ( VmbErrorSuccess != res )
        {
            // If anything fails after opening the camera we close it
            m_pCamera->Close();
        }
    }

    return res;
}

/**setting a feature to maximum value that is a multiple of 2 and a multiple of the increment*/
VmbErrorType SetIntFeatureValueModulo2( const CameraPtr &pCamera, const char* const& Name )
{
    VmbErrorType    result;
    FeaturePtr      feature;
    VmbInt64_t      value_min = 0;
    VmbInt64_t      value_max = 0;
    VmbInt64_t      value_increment = 0;

    result = SP_ACCESS( pCamera )->GetFeatureByName( Name, feature );
    if( VmbErrorSuccess != result )
    {
        return result;
    }

    result = SP_ACCESS( feature )->GetRange( value_min, value_max );
    if( VmbErrorSuccess != result )
    {
        return result;
    }

    result = SP_ACCESS( feature )->GetIncrement( value_increment );
    if( VmbErrorSuccess != result )
    {
        return result;
    }

    value_max = value_max - ( value_max % value_increment);
    if( value_max % 2 != 0)
    {
        value_max -= value_increment;
    }

    result = SP_ACCESS( feature )->SetValue ( value_max );
    return result;
}

/**prepare camera so that the delivered image will not fail in image transform*/
VmbErrorType ApiController::PrepareCamera()
{
    VmbErrorType result;
    result = SetIntFeatureValueModulo2( m_pCamera, "Width" );
    if( VmbErrorSuccess != result )
    {
        return result;
    }
    result = SetIntFeatureValueModulo2( m_pCamera, "Height" );
    if( VmbErrorSuccess != result )
    {
        return result;
    }
    return result;
}

//
// Calls the API convenience function to stop image acquisition
// Closes the camera
//
// Returns:
//  An API status code
//
VmbErrorType ApiController::StopContinuousImageAcquisition()
{
    // Stop streaming
    m_pCamera->StopContinuousImageAcquisition();

    // Close camera
    return  m_pCamera->Close();
}

//
// Gets all cameras known to Vimba
//
// Returns:
//  A vector of camera shared pointers
//
CameraPtrVector ApiController::GetCameraList() const
{
    CameraPtrVector cameras;
    // Get all known cameras
    if ( VmbErrorSuccess == m_system.GetCameras( cameras ))
    {
        // And return them
        return cameras;
    }
    return CameraPtrVector();
}

//
// Gets the version of the Vimba API
//
// Returns:
//  The version as string
//
std::string ApiController::GetVersion() const
{
    std::ostringstream  os;
    os<<m_system;
    return os.str();
}

}} // namespace AVT::VmbAPI