#include <string>
#include <cstring>
#include <iostream>

#include "VimbaCPP/Include/VimbaCPP.h"
#include "ApiController.h"

int main( int argc, char* argv[] )
{
    VmbErrorType err = VmbErrorSuccess;

    std::cout<<"///////////////////////////////////////////\n";
    std::cout<<"/// Vimba API Asynchronous Grab Example ///\n";
    std::cout<<"///////////////////////////////////////////\n\n";

    //////////////////////
    //Parse command line//
    //////////////////////
    AVT::VmbAPI::ProgramConfig Config;
    err = Config.ParseCommandline( argc, argv);
    //Write out an error if we could not parse the command line
    if ( VmbErrorBadParameter == err )
    {
        std::cout<< "Invalid parameters!\n\n" ;
        Config.setPrintHelp( true );
    }

    //Print out help and end program
    if ( Config.getPrintHelp() )
    {
        Config.PrintHelp( std::cout );
    }
    else
    {
        AVT::VmbAPI::ApiController apiController;
        
        // Print out version of Vimba
        std::cout << "Vimba C++ API Version " << apiController.GetVersion() << "\n";
        
        // Startup Vimba
        err = apiController.StartUp();        
        if ( VmbErrorSuccess == err )
        {
            if( Config.getCameraID().empty() )
            {
                AVT::VmbAPI::CameraPtrVector cameras = apiController.GetCameraList();
                if( cameras.empty() )
                {
                    err = VmbErrorNotFound;
                }
                else
                {
                    std::string strCameraID;
                    err = cameras[0]->GetID( strCameraID );
                    if( VmbErrorSuccess == err )
                    {
                        Config.setCameraID( strCameraID );
                    }
                }
            }
            if ( VmbErrorSuccess == err )
            {
                std::cout<<"Opening camera with ID: "<< Config.getCameraID() <<"\n";

                err = apiController.StartContinuousImageAcquisition( Config );

                if ( VmbErrorSuccess == err )
                {
                    std::cout<< "Press <enter> to stop acquisition...\n" ;
                    getchar();

                    apiController.StopContinuousImageAcquisition();
                }
            }

            apiController.ShutDown();
        }

        if ( VmbErrorSuccess == err )
        {
            std::cout<<"\nAcquisition stopped.\n" ;
        }
        else
        {
            std::string strError = apiController.ErrorCodeToMessage( err );
            std::cout<<"\nAn error occurred: " << strError << "\n";
        }
    }

    return err;
}
