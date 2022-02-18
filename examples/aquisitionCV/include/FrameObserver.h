#ifndef AVT_VMBAPI_EXAMPLES_FRAMEOBSERVER
#define AVT_VMBAPI_EXAMPLES_FRAMEOBSERVER

#include <queue>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "ProgramConfig.h"
#include "FrameProcessing.h"

namespace AVT {
namespace VmbAPI {

class FrameObserver : virtual public IFrameObserver
{
    public:
        /**
         * @brief Construct a new Frame Observer object
         * 
         * @param pCamera The camera the frame was queued at
         * @param eFrameInfos Indicates how the frame will be displayed
         * @param eColorProcessing Indicates how color processing is applied
         * @param bRGBValue 
         */
        FrameObserver( CameraPtr pCamera, FrameInfos eFrameInfos, ColorProcessing eColorProcessing, bool bRGBValue );
        
        /**
         * @brief This is our callback routine that will be executed on every received frame.
         * Triggered by the API
         * 
         * @param pFrame The frame returned from the API
         */
        virtual void FrameReceived( const FramePtr pFrame );

    private:
        void ShowFrameInfos( const FramePtr & );
        double GetTime();

        template <typename T>
        class ValueWithState
        {
            private:
                T m_Value;
                bool m_State;
            public:
                ValueWithState()
                    : m_State( false )
                {}
                ValueWithState( T &value )
                    : m_Value ( value )
                    , m_State( true )
                {}
                const T& operator()() const
                {
                    return m_Value;
                }
                void operator()( const T &value )
                {
                    m_Value = value;
                    m_State = true;
                }
                bool IsValid() const
                {
                    return m_State;
                }
                void Invalidate()
                {
                    m_State = false;
                }
        };

        const FrameInfos            m_eFrameInfos;
        const bool                  m_bRGB;
        const ColorProcessing       m_eColorProcessing;
        ValueWithState<double>      m_FrameTime;
        ValueWithState<VmbUint64_t> m_FrameID;
        FrameProcessing *           proc;
};

}} // namespace AVT::VmbAPI

#endif
