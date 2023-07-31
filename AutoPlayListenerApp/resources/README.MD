## Resources

A few registry examples of how it can be configured in the Windows Registry system to ensure proper auto play handlers are set.

1. AutoPlayHandlers_progid_registry.reg - the progId registry which points to where the CLSID class has been registered
2. AutoPlayHandlers_custom_device_handler_registry.reg - the AutoPlay custom device (Phone connection/disconection) handler created to react from 
3. AutoPlayHandlers_clsid_class_registry.reg - The result of running the CLI tool to setup the registry. This is needed to ensure the proper win32 server is used to send commands