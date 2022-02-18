#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "ListCameras.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

namespace AVT {
namespace VmbAPI {

/**
 * @brief Print single camera information
 * 
 * @param camera pointer to the camera
 */
void PrintCameraInfo( const CameraPtr &camera )
{
    std::string strID;
    std::string strName;
    std::string strModelName;
    std::string strSerialNumber;
    std::string strInterfaceID;

    std::ostringstream ErrorStream;

    camera->GetID( strID );    
    camera->GetName( strName );
    camera->GetModel( strModelName );
    camera->GetSerialNumber( strSerialNumber );
    camera->GetInterfaceID( strInterfaceID );

    std::cout   << "/// Camera Name    : " << strName           << "\n"
                << "/// Model Name     : " << strModelName      <<  "\n"
                << "/// Camera ID      : " << strID             <<  "\n"
                << "/// Serial Number  : " << strSerialNumber   <<  "\n"
                << "/// @ Interface ID : " << strInterfaceID    << "\n\n";
}

/**
 * @brief Start Vimba and obtain connected cameras
 * 
 */
void ListCameras::Print()
{
    VimbaSystem& sys = VimbaSystem::GetInstance();     // Get a reference to the VimbaSystem singleton
    std::cout<<"Vimba C++ API Version "<<sys<<"\n";    // Print out version of Vimba

    VmbErrorType    err = sys.Startup();               // Initialize the Vimba API
    CameraPtrVector cameras;                           // A vector of std::shared_ptr<AVT::VmbAPI::Camera> objects

    std::stringstream strError;

    if( VmbErrorSuccess == err ) {
        err = sys.GetCameras( cameras );               // Fetch all cameras known to Vimba
        if( VmbErrorSuccess == err ) {
            std::cout << "Cameras found: " << cameras.size() <<"\n\n";

            // Query all static details of all known cameras and print them out.
            std::for_each( cameras.begin(), cameras.end(), PrintCameraInfo );  
        } else {
            std::cout << "Could not list cameras. Error code: " << err << "("<<AVT::VmbAPI::ErrorCodeToMessage(err)<<")"<< "\n";
        }

        sys.Shutdown();                                // Close Vimba
    } else {
        std::cout << "Could not start system. Error code: " << err <<"("<<AVT::VmbAPI::ErrorCodeToMessage(err)<<")"<< "\n";
    }
}

}} // namespace AVT::VmbAPI