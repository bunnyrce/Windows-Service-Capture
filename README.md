# Windows Service Background Capture - The Sneaky Background Bastard
<!-- //madebybunnyrce -->

Native C++ Windows service that runs persistently in the background using Service Control Manager APIs. Demonstrates service registration, control handling, and background operation techniques. Survives reboots and user logoffs by integrating with Windows service management. //madebybunnyrce

## How This Background Service Works

This implementation creates a persistent Windows service that just won't fuck off:

- **Service Registration**: Uses Service Control Manager APIs to install this bastard permanently
- **Service Control**: Handles start, stop, and status reporting like a well-behaved service should
- **Background Operation**: Runs continuously in the background without bothering the user
- **System Integration**: Integrates with Windows service management so it looks legit

This shit survives reboots, user logoffs, and pretty much everything except manual removal.

## Files in This Package

- `service.cpp` - Main Windows service implementation
- `capture.cpp` - Background capture functionality
- `install.bat` - Service installation script
- `uninstall.bat` - Service removal script
- `build.bat` - Build script for service compilation
- `config.ini` - Service configuration settings
- `README.md` - This documentation file

## How to Use This Service

1. **Compile the service:**
   ```cmd
   build.bat
   ```
2. **Install service (as Administrator):**
   ```cmd
   install.bat
   ```
3. **Start the service:**
   ```cmd
   net start CaptureService
   ```
4. **Stop and uninstall:**
   ```cmd
   net stop CaptureService
   uninstall.bat
   ```

## Technical Details

This implementation demonstrates Windows service techniques:

- **CreateService()** - Registers service with Service Control Manager
- **StartServiceCtrlDispatcher()** - Service entry point and dispatcher
- **SetServiceStatus()** - Reports service status to SCM
- **RegisterServiceCtrlHandler()** - Handles service control requests
- **Service main loop** - Background processing and resource management

## API Signatures

This implementation uses these Windows Service APIs:
- OpenSCManager() for Service Control Manager access
- CreateService() for service registration
- StartServiceCtrlDispatcher() for service execution
- SetServiceStatus() for status reporting
- RegisterServiceCtrlHandler() for control handling

## Requirements

- Windows 10/11 with service support
- Visual Studio with Windows SDK
- Administrator privileges for service installation
- Service Control Manager access rights

## Limitations

- **Administrator required** - service installation needs elevated privileges
- **System service** - runs in system context with limited user interaction
- **Persistent operation** - continues running until explicitly stopped
- **Resource constraints** - must manage memory and handles carefully
- **Security context** - runs with service account privileges

## Usage Notes

This is for **educational and development purposes**. Windows services are used by system monitoring tools, backup software, and administrative applications for background processing.

---

**Note**: Windows service APIs require administrator privileges and create persistent system processes that are commonly monitored by security software.