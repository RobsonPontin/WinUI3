#pragma once

namespace COMServerApp
{
	class LifetimeManager : public std::enable_shared_from_this<LifetimeManager>
	{
	public:
		LifetimeManager() = default;
		~LifetimeManager() = default;

		static std::shared_ptr<LifetimeManager> CreateShared();
		HANDLE GetSHutdownEventHandle();

		/// <summary>
		/// Increment ref count.
		/// </summary>
		void Lock();

		/// <summary>
		/// Decrement Ref count.
		/// </summary>
		void Unlock();

		void RegisterComClass();

	private:
		wil::unique_event m_shutdownEvent{ wil::EventOptions::None };
		wil::unique_com_class_object_cookie m_classCookie;
		static std::shared_ptr<LifetimeManager> m_lifetimeManager;

		int m_refCount{ 0 };
	};
}
