#include "pch.h"
#include "TestVideoApis.h"

#include <iostream>

#include <winrt/Windows.Media.Core.h>

#include "DebugLog.h"

namespace Playground
{
	void TestMediaPlayerApis::Initialize()
	{
		if (m_mediaPlayer != nullptr)
		{
			return;
		}

		m_mediaPlayer = WMP::MediaPlayer();
		m_mediaPlayerElement = WUXC::MediaPlayerElement();
		m_mediaPlayerElement.AutoPlay(false);

		m_mediaPlayer.MediaFailed([](auto&&, auto&& args)
			{
				std::wcout << L"MediaFailed: " << args.ErrorMessage().c_str() << std::endl;
			});
	}

	WF::IAsyncAction TestMediaPlayerApis::LoadVideoFileAsync(WS::StorageFile file)
	{
		Initialize();

		if (m_playbackItem != nullptr)
		{
			m_playbackItem = nullptr;
		}

		auto videoMediaSource = WMC::MediaSource::CreateFromStorageFile(file);
		videoMediaSource.StateChanged([](WMC::MediaSource const& sender, WMC::MediaSourceStateChangedEventArgs const& args)
			{
				std::wcout << L"MediaSource StateChanged: " << static_cast<int>(args.NewState()) << std::endl;
			});

		// NOTE: there are 2 important components to the media player:
		// 1. MediaPlayerElement (XAML control) - the media player UI which will render the video
		// 2. MediaPlayer (playback engine) - the media player core which will handle the playback
		// 
		// Approach 1
		// When we are using the "MediaSource" in the MediaPlayerElement, it's job will be to render the video
		// When we are using the "MediaPlaybackItem" in the MediaPlayer, it's job will be to handle the playback - play, pause, seek, etc.
		// Approach 2
		// When we are using the "MediaPlaybackItem" in the MediaPlayerElement, it's job will be to render the video and handle the playback


		m_mediaPlayerElement.Source(videoMediaSource);

		auto videoMediaSource2 = WMC::MediaSource::CreateFromStorageFile(file);
		
		co_await videoMediaSource2.OpenAsync();
		m_playbackItem = WMP::MediaPlaybackItem(videoMediaSource2);
		
		if (m_playbackItem.VideoTracks().Size() > 0)
		{
			std::cout << "Video tracks available!" << std::endl;
		}

		m_playbackItem.VideoTracksChanged([](WMP::MediaPlaybackItem item, WFC::IVectorChangedEventArgs tracks)
			{
				if (tracks.CollectionChange() == WFC::CollectionChange::ItemInserted)
				{
					auto videoTracks = item.VideoTracks();
					auto track = videoTracks.GetAt(tracks.Index());
					std::cout << "Video tracks: Item has been inserted!" << std::endl;
				}
			});
		m_playbackItem.AudioTracksChanged([](auto&&, auto&&)
			{
				std::cout << "Hello, Windows console!" << std::endl;
			});


		m_mediaPlayer = m_mediaPlayerElement.MediaPlayer();

		co_return;
	}
}