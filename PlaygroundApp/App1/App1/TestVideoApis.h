#pragma once

#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

#include "D3DResources.h"

namespace Playground
{
	struct TestMediaPlayerApis : std::enable_shared_from_this<TestMediaPlayerApis>
	{
		TestMediaPlayerApis() = default;

		WF::IAsyncAction LoadVideoFileAsync(WS::StorageFile file);

		WF::IAsyncOperation<WS::Streams::InMemoryRandomAccessStream> ExtractFrameFromVideoAsync(WS::StorageFile file, WF::TimeSpan playbackPosition);
		WF::IAsyncAction RequestFrameFromVideoAsync(WS::StorageFile file, WF::TimeSpan playbackPosition);

		// Events
		winrt::event_token FrameImageReady(winrt::delegate<WGI::SoftwareBitmap> const& handler);

	private:
		// MediaPlayer
		WMP::MediaPlaybackItem m_playbackItem{ nullptr }; // Only keeping this one as a member so we can listen to events
		WMP::MediaPlayer m_mediaPlayer{ nullptr };
		WUXC::MediaPlayerElement m_mediaPlayerElement{ nullptr };

		void InitializeMediaPlayer();

		// MediaPlayer as Video Frame Server
		WMP::MediaPlayer m_mediaPlayerVideoFrameServer{ nullptr };
		winrt::apartment_context m_uiContext{};
		WMP::MediaPlayer::VideoFrameAvailable_revoker m_videoFrameAvailableRevoker{};

		void InitializeMediaPlayerVideoFrameServer();
		WF::IAsyncAction MediaPlayerVideoFrameServer_VideoFrameAvailable(WMP::MediaPlayer const& sender, WF::IInspectable const& args);

		// Events
		winrt::event<winrt::delegate<WGI::SoftwareBitmap>> m_frameImageReady;
		void FrameImageReadyRemoveToken(winrt::event_token const& token) { m_frameImageReady.remove(token); }
	};
}
