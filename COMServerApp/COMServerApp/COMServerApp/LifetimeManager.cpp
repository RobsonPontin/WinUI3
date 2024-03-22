#include "pch.h"

#include "LifetimeManager.h"
#include "COMServerAppSimpleClass.h"


namespace COMServerApp
{
	std::shared_ptr<LifetimeManager> LifetimeManager::m_lifetimeManager;

	std::shared_ptr<LifetimeManager> LifetimeManager::CreateShared()
	{
		if (m_lifetimeManager == nullptr)
		{
			m_lifetimeManager = std::make_shared<LifetimeManager>();
		}

		return m_lifetimeManager;
	}

	HANDLE LifetimeManager::GetSHutdownEventHandle()
	{
		return m_shutdownEvent.get();
	}

	void LifetimeManager::RegisterComClass()
	{
		// Register all COM classes in this project, in this case only 1.
		wil::unique_com_class_object_cookie regCookie;
		winrt::check_hresult(::CoRegisterClassObject(
			CLSID_COMServerAppSimpleClass,
			winrt::make<COMServerAppSimpleClass_Factory>().get(),
			CLSCTX_LOCAL_SERVER,
			REGCLS_SINGLEUSE,
			&regCookie));

		m_classCookie = std::move(regCookie);
	}

	void LifetimeManager::Lock()
	{
		m_refCount++;
	}

	void LifetimeManager::Unlock()
	{
		m_refCount--;

		if (m_refCount <= 0)
		{
			m_shutdownEvent.SetEvent();
		}
	}
}