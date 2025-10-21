#pragma once

#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

namespace Playground
{
	struct TestMediaPlayerApis
	{
		TestMediaPlayerApis() = default;

		WF::IAsyncAction LoadVideoFileAsync(WS::StorageFile file);

		WF::IAsyncOperation<WS::Streams::InMemoryRandomAccessStream> ExtractFrameFromVideoAsync(WS::StorageFile file, WF::TimeSpan playbackPosition);

	private:
		WMP::MediaPlaybackItem m_playbackItem{ nullptr };
		WMP::MediaPlayer m_mediaPlayer{ nullptr };
		WUXC::MediaPlayerElement m_mediaPlayerElement{ nullptr };

		void Initialize();
	};
}
