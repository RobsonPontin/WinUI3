#include "pch.h"
#include "TestVideoApis.h"

#include <iostream>

#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Editing.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.h>

#include "DebugLog.h"

namespace Playground
{
	void TestMediaPlayerApis::InitializeMediaPlayer()
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

	void TestMediaPlayerApis::InitializeMediaPlayerVideoFrameServer(int width, int height)
	{
		if (m_mediaPlayerVideoFrameServer != nullptr)
		{
			return;
		}
		
		m_mediaPlayerVideoFrameServer = WMP::MediaPlayer();
		m_mediaPlayerVideoFrameServer.IsVideoFrameServerEnabled(true);

		m_videoFrameAvailableRevoker = m_mediaPlayerVideoFrameServer.VideoFrameAvailable(
			winrt::auto_revoke, { shared_from_this(), &TestMediaPlayerApis::MediaPlayerVideoFrameServer_VideoFrameAvailable });

		m_d3d = std::make_unique<D3DResources>(D3DResources::Create(width, height));
	}

	WF::IAsyncAction TestMediaPlayerApis::LoadVideoFileAsync(WS::StorageFile file)
	{
		InitializeMediaPlayer();

		if (m_playbackItem != nullptr)
		{
			m_playbackItem = nullptr;
		}

		auto videoMediaSource = WMC::MediaSource::CreateFromStorageFile(file);
		videoMediaSource.StateChanged([](WMC::MediaSource const&, WMC::MediaSourceStateChangedEventArgs const& args)
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

	WF::IAsyncOperation<WS::Streams::InMemoryRandomAccessStream> TestMediaPlayerApis::ExtractFrameFromVideoAsync(WS::StorageFile file, WF::TimeSpan playbackPosition)
	{
		try
		{
			// Set up media composition from the current one-up video for frame extraction
			auto mediaClip = co_await winrt::Windows::Media::Editing::MediaClip::CreateFromFileAsync(file);
			auto videoDuration = mediaClip.OriginalDuration();

			if (videoDuration < playbackPosition)
			{
				co_return nullptr;
			}

			auto mediaComposition{ winrt::Windows::Media::Editing::MediaComposition() };
			mediaComposition.Clips().Append(mediaClip);

			// When playback position is near the end of video, GetThumbnailAsync() in frame extraction would recognize the playbacktime as "out of bound"
			// Thus, we manually make sure the value is smaller than the end time by 0.1 millisecond to make sure the frams can be extracted
			playbackPosition = std::chrono::floor<std::chrono::milliseconds>(playbackPosition);
			if (playbackPosition > std::chrono::floor<std::chrono::milliseconds>(videoDuration) - std::chrono::milliseconds(100))
			{
				playbackPosition -= std::chrono::milliseconds(100);
			}

			// Use default values for width and height
			int32_t width = 0;
			int32_t height = 0;

			auto imageStream = co_await mediaComposition.GetThumbnailAsync(
				playbackPosition,
				width,
				height,
				winrt::Windows::Media::Editing::VideoFramePrecision::NearestFrame);

			WS::Streams::InMemoryRandomAccessStream randomAccessStream;
			co_await WS::Streams::RandomAccessStream::CopyAsync(imageStream, randomAccessStream);
			randomAccessStream.Seek(0);
			co_return randomAccessStream;
		}
		catch (...)
		{
			co_return nullptr;
		}
	}

	WF::IAsyncAction TestMediaPlayerApis::RequestFrameFromVideoAsync(WS::StorageFile file, WF::TimeSpan playbackPosition, int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			throw std::invalid_argument("Invalid video frame size");
		}

		// Target size for video frame
		InitializeMediaPlayerVideoFrameServer(width, height);

		auto videoMediaSource = WMC::MediaSource::CreateFromStorageFile(file);
		co_await videoMediaSource.OpenAsync();

		m_mediaPlayerVideoFrameServer.Source(videoMediaSource);
	}

	WF::IAsyncAction TestMediaPlayerApis::MediaPlayerVideoFrameServer_VideoFrameAvailable(WMP::MediaPlayer const& sender, WF::IInspectable const&)
	{
		// The event will be raised on a background thread, so we need to marshal back to the UI thread if necessary
		co_await m_uiContext;

		try
		{
			auto frameRect = m_d3d->GetFrameSize();

			// Copy the current video frame
			sender.CopyFrameToVideoSurface(m_d3d->m_frameSurface, frameRect);

			WGI::SoftwareBitmap sb = co_await WGI::SoftwareBitmap::CreateCopyFromSurfaceAsync(
				m_d3d->m_frameSurface,
				WGI::BitmapAlphaMode::Premultiplied);

			m_frameImageReady(sb);
		}
		catch (...)
		{

		}
	}

	winrt::event_token TestMediaPlayerApis::FrameImageReady(winrt::delegate<WGI::SoftwareBitmap> const& handler)
	{
		return m_frameImageReady.add(handler);
	}
}