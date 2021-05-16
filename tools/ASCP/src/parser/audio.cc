#include "audio.hh"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

void AudioParser::performParse( const std::string_view &svFileName,
                                Kokoro::Memory::Buffer &bInputBuffer )
{
    std::string_view extension =
        svFileName.substr( svFileName.rfind( '.' ) + 1 );

    auto &data = bInputBuffer.data();

    drwav wav;
    if ( !drwav_init_memory( &wav, data.data(), data.size(), NULL ) )
    {
        std::cout << "Failed to parse: " << svFileName;
        return;
    }

    std::vector<uint8_t> pcmFrames;
    pcmFrames.resize( wav.totalPCMFrameCount * wav.channels
                      * ( wav.bitsPerSample / 8 ) );

    size_t numberOfSamplesActuallyDecoded = drwav_read_pcm_frames(
        &wav, wav.totalPCMFrameCount, (void *) pcmFrames.data() );

    MOVE_TO_HEAP( int, frequency, wav.sampleRate );
    setKData( "frequency", HEAP_frequency );

    if ( wav.channels > 1 && wav.channels <= 2 )
    {
        // Stereo, seperate audio channels
        std::vector<uint8_t> pcmFramesLeft;
        std::vector<uint8_t> pcmFramesRight;

        uint32_t bytesPerSample = wav.bitsPerSample / 8;

        for ( auto i = 0; i < pcmFrames.size(); i += ( bytesPerSample * 2 ) )
        {
            // Left
            for ( auto x = 0; x < bytesPerSample; x += 1 )
            {
                pcmFramesLeft.push_back( pcmFrames.at( i + x ) );
            }

            // Right
            for ( auto x = bytesPerSample; x < ( bytesPerSample * 2 ); x += 1 )
            {
                pcmFramesLeft.push_back( pcmFrames.at( i + x ) );
            }
        }

        MOVE_TO_HEAP( int, isMono, false );
        MOVE_TO_HEAP( std::vector<uint8_t>, channelLeftData, pcmFramesLeft );
        MOVE_TO_HEAP( std::vector<uint8_t>, channelRightData, pcmFramesRight );

        setKData( "isMono", HEAP_isMono );
        setKData( "channelLeftData", HEAP_channelLeftData );
        setKData( "channelRightData", HEAP_channelRightData );
    }
    else
    {
        // Mono, no pre-processing needed.
        MOVE_TO_HEAP( int, isMono, true );
        MOVE_TO_HEAP( std::vector<uint8_t>, channelLeftData, pcmFrames );
        MOVE_TO_HEAP( std::vector<uint8_t>, channelRightData,
                      std::vector<uint8_t>() );  // Empty, just mono

        setKData( "isMono", HEAP_isMono );
        setKData( "channelLeftData", HEAP_channelLeftData );
        setKData( "channelRightData", HEAP_channelRightData );
    }

    drwav_uninit( &wav );
}
