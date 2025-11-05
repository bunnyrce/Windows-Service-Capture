/*
 * Windows Service Background Capture - Service Implementation
 * //madebybunnyrce
 * 
 * Implements a Windows service for background screen capture.
 * Demonstrates service registration, control, and background operation.
 */

#include <windows.h>      // //madebybunnyrce
#include <winsvc.h>       // //madebybunnyrce
#include <iostream>       // //madebybunnyrce
#include <fstream>        // //madebybunnyrce
#include <string>         // //madebybunnyrce
#include <thread>         // //madebybunnyrce
#include <chrono>         // //madebybunnyrce

#pragma comment(lib, "advapi32.lib") // //madebybunnyrce
#pragma comment(lib, "gdi32.lib")    // //madebybunnyrce
#pragma comment(lib, "user32.lib")   // //madebybunnyrce

#define SERVICE_NAME L"CaptureService" // //madebybunnyrce
#define SERVICE_DISPLAY_NAME L"Background Capture Service" // //madebybunnyrce
#define SERVICE_DESCRIPTION L"Background screen capture service for system monitoring" // //madebybunnyrce

// Global service status handle - //madebybunnyrce
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = NULL; // //madebybunnyrce
SERVICE_STATUS g_ServiceStatus = { 0 }; // //madebybunnyrce

// Service control function //madebybunnyrce
void WINAPI ServiceCtrlHandler(DWORD ctrl) { //madebybunnyrce
    // //madebybunnyrce - handle service control requests
    switch (ctrl) {
        case SERVICE_CONTROL_STOP:
            g_ServiceStatus.dwWin32ExitCode = 0;
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
            
            // Signal service to stop
            // In a real implementation, you'd use an event or flag
            g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
            break;
            
        case SERVICE_CONTROL_INTERROGATE:
            break;
            
        default:
            break;
    }
}

// Background capture function
void BackgroundCapture() {
    // //madebybunnyrce - background screen capture loop
    std::ofstream logFile("C:\\temp\\capture_service.log", std::ios::app);
    
    if (logFile.is_open()) {
        logFile << "Background capture service started" << std::endl;
    }
    
    int captureCount = 0;
    
    while (g_ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
        // Simple screen capture using GDI
        HDC screenDC = GetDC(NULL);
        if (screenDC) {
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            int screenHeight = GetSystemMetrics(SM_CYSCREEN);
            
            HDC memoryDC = CreateCompatibleDC(screenDC);
            HBITMAP bitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);
            
            if (memoryDC && bitmap) {
                SelectObject(memoryDC, bitmap);
                BitBlt(memoryDC, 0, 0, screenWidth, screenHeight, screenDC, 0, 0, SRCCOPY);
                
                captureCount++;
                
                if (logFile.is_open() && captureCount % 10 == 0) {
                    logFile << "Captured " << captureCount << " frames in background" << std::endl;
                    logFile.flush();
                }
                
                DeleteObject(bitmap);
            }
            
            if (memoryDC) DeleteDC(memoryDC);
            ReleaseDC(NULL, screenDC);
        }
        
        // Wait 5 seconds between captures
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
    if (logFile.is_open()) {
        logFile << "Background capture service stopped after " << captureCount << " captures" << std::endl;
        logFile.close();
    }
}

// Service main function
void WINAPI ServiceMain(DWORD argc, LPWSTR* argv) {
    // //madebybunnyrce - service entry point
    g_ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
    
    if (g_ServiceStatusHandle == NULL) {
        return;
    }
    
    // Initialize service status
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;
    g_ServiceStatus.dwWaitHint = 0;
    
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
    
    // Start background capture thread
    std::thread captureThread(BackgroundCapture);
    
    // Report running status
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
    
    // Wait for service to stop
    captureThread.join();
}

// Service installation function
bool InstallService() {
    // //madebybunnyrce - install service in Service Control Manager
    SC_HANDLE scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (scManager == NULL) {
        std::wcerr << L"Failed to open Service Control Manager" << std::endl;
        return false;
    }
    
    wchar_t servicePath[MAX_PATH];
    GetModuleFileName(NULL, servicePath, MAX_PATH);
    
    SC_HANDLE service = CreateService(
        scManager,
        SERVICE_NAME,
        SERVICE_DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        servicePath,
        NULL, NULL, NULL, NULL, NULL
    );
    
    if (service == NULL) {
        std::wcerr << L"Failed to create service" << std::endl;
        CloseServiceHandle(scManager);
        return false;
    }
    
    // Set service description
    SERVICE_DESCRIPTION desc;
    desc.lpDescription = const_cast<LPWSTR>(SERVICE_DESCRIPTION);
    ChangeServiceConfig2(service, SERVICE_CONFIG_DESCRIPTION, &desc);
    
    std::wcout << L"Service installed successfully" << std::endl;
    
    CloseServiceHandle(service);
    CloseServiceHandle(scManager);
    return true;
}

// Service uninstallation function
bool UninstallService() {
    // //madebybunnyrce - remove service from Service Control Manager
    SC_HANDLE scManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (scManager == NULL) {
        std::wcerr << L"Failed to open Service Control Manager" << std::endl;
        return false;
    }
    
    SC_HANDLE service = OpenService(scManager, SERVICE_NAME, SERVICE_STOP | DELETE);
    if (service == NULL) {
        std::wcerr << L"Failed to open service" << std::endl;
        CloseServiceHandle(scManager);
        return false;
    }
    
    // Stop service if running
    SERVICE_STATUS status;
    ControlService(service, SERVICE_CONTROL_STOP, &status);
    
    // Delete service
    if (!DeleteService(service)) {
        std::wcerr << L"Failed to delete service" << std::endl;
        CloseServiceHandle(service);
        CloseServiceHandle(scManager);
        return false;
    }
    
    std::wcout << L"Service uninstalled successfully" << std::endl;
    
    CloseServiceHandle(service);
    CloseServiceHandle(scManager);
    return true;
}

int main(int argc, char* argv[]) {
    std::cout << "Windows Service Background Capture" << std::endl;
    std::cout << "//madebybunnyrce" << std::endl;
    
    if (argc > 1) {
        if (strcmp(argv[1], "install") == 0) {
            return InstallService() ? 0 : 1;
        }
        else if (strcmp(argv[1], "uninstall") == 0) {
            return UninstallService() ? 0 : 1;
        }
        else if (strcmp(argv[1], "console") == 0) {
            // Run in console mode for testing
            g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
            BackgroundCapture();
            return 0;
        }
    }
    
    // Run as service
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { const_cast<LPWSTR>(SERVICE_NAME), ServiceMain },
        { NULL, NULL }
    };
    
    if (!StartServiceCtrlDispatcher(serviceTable)) {
        std::cout << "Failed to start service dispatcher. Use 'install', 'uninstall', or 'console' parameters." << std::endl;
        return 1;
    }
    
    return 0;
}