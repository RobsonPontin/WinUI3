## Resources

A few registry examples of how it can be configured in the Windows Registry system to ensure proper auto play handlers are set.

1. hk_cu_autoplayhandlers_handler - The actual handler registered for the Application which will point to the handler ProgId
2. hk_root_autoplayapp.handler - ProgId which redirects to the COM LocalServer32
3. hk_root_autoplayapp.handler.1 - ProgId which redirects to the COM LocalServer32
4. hkey_root_clsid_LocalServer32_class - the actual COM server registration