// library for usb functionality
#pragma once

#define BOARD_ID "Patient_Sim_Board"

// #include "global_constants.h"

#include <string.h>
#include <iostream> 
#include <vector> 

#include <windows.h>
#include <SetupAPI.h>
#include <initguid.h> // Include before devpkey.h
#include <devpkey.h>
#include <WinIoCtl.h>


class USBManager {
    private:
        HANDLE usb_handle = nullptr;

    public:
        ~USBManager() {
            CloseConnection();
        }
    
        bool IsConnected() { return usb_handle != nullptr; }
        explicit operator bool() const { return usb_handle != nullptr; }
 
        bool AutoConnect();
        bool OpenConnection(std::string device);
        void CloseConnection() {
            CloseHandle(usb_handle);
            usb_handle = nullptr;
        }

        DWORD write_raw(unsigned char* write_array, int num = 8);
        DWORD read_raw(unsigned char* read_array, int num = 8);
        
        void ClearInputBuffer();
        void ClearOutputBuffer();
};

bool USBManager::AutoConnect(){
    if (this->IsConnected()) {
        return true;
    }
    CloseConnection();

    std::string device_target;
    std::string board_id = BOARD_ID;

    HDEVINFO devices = SetupDiGetClassDevsA(&GUID_DEVINTERFACE_COMPORT, // GUID_DEVINTERFACE_COMPORT: retrieve comm ports 
                                            0,
                                            0,
                                            DIGCF_DEVICEINTERFACE | DIGCF_PRESENT); // DIGCF_DEVICEINTERFACE: device interfaces
                                                                 // DIGCF_PRESENT: only present devices

    if (devices == INVALID_HANDLE_VALUE) {
        return false; // return nullptr if nothing connected
    }

    std::string description;
    for (DWORD device_index = 0; ; device_index++)
    {
        if (GetLastError() == ERROR_NO_MORE_ITEMS) {
            break; // No more devices to enumerate
        }

        {
            SP_DEVINFO_DATA device_info = {};
            device_info.cbSize = sizeof(SP_DEVINFO_DATA);
            if (!SetupDiEnumDeviceInfo(devices, device_index, &device_info)) {
                continue; // Insert error handling here
            }

            std::vector<WCHAR> buffer(1024);

            // You may call this with zero length to get the required size
            DWORD property_type;
            DWORD required_size = 0;
            if (!SetupDiGetDevicePropertyW(
                devices,
                &device_info,
                &DEVPKEY_Device_BusReportedDeviceDesc,
                &property_type,
                (PBYTE)buffer.data(),
                (DWORD)buffer.size(),
                &required_size,
                0)) {
                continue; // Insert error handling here
            }
        description = std::string(buffer.begin(), buffer.end());
        }
    
        {
            SP_DEVICE_INTERFACE_DATA interface_data = {};
            interface_data.cbSize = sizeof(interface_data);
            if (!SetupDiEnumDeviceInterfaces(
                devices,
                0,
                &GUID_DEVINTERFACE_COMPORT,
                device_index,
                &interface_data)) {
                continue; // Insert error handling here
            }

            std::vector<char> buffer(1024);

            auto details = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)buffer.data();
            details->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

            // You may call this with zero length to get the required size
            DWORD required_size = 0;
            if (!SetupDiGetDeviceInterfaceDetailA(
                devices,
                &interface_data,
                details,
                buffer.size(),
                &required_size,
                0)) {
                continue; // Insert error handling here
            }

            std::cerr << "device: " << description.c_str() << std::endl;
            device_target = details->DevicePath;

            // // check if the description matches the board_id
            // if (description.c_str() != '\n') {
            //     std::cerr << board_id << " found!\nUSB address: " << details->DevicePath << "\n\n";
            //     device_target = details->DevicePath;
            //     break;  // stop checking for other devices
            // }
        }

    }

    SetupDiDestroyDeviceInfoList(devices);
    
    if (device_target.empty()) {     // if a device was found, try connecting
        std::cerr << "No devices found!\n\n";
        this->usb_handle = nullptr;
        return false;
    } else {                         // else, launch in standalone
        return OpenConnection(device_target);
    }
}

bool USBManager::OpenConnection(std::string device){
       HANDLE handle = CreateFileA(device.c_str(),
            GENERIC_READ | GENERIC_WRITE, // Desired access
            0,              // Share mode
            nullptr,        // Security attributes
            OPEN_EXISTING,  // Creation disposition
            0,              // Flags and attributes (0 for synchronous I/O)
            nullptr);       // Template file

        if (!handle || handle == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening hardware at port: " << device << "\n\n"; // Insert error handling here
            this->usb_handle = nullptr;
            return false;
        } else {
            std::cerr << "Connection to device successful!\n\n";
            this->usb_handle = handle;
            return true;
        }
}


DWORD USBManager::write_raw(unsigned char* write_array, int num){

        DWORD bytes_transferred;						// number of bytes that have been transferred 
		if (!(WriteFile(usb_handle, write_array, num, &bytes_transferred, nullptr))) {
			std::cerr << "Error: Failed to send data.\n"; // if it failed, cerr out an error message 
            std::cerr << "Attempting reconnect...\n";
		}	
        FlushFileBuffers(usb_handle);
        return bytes_transferred;
}

DWORD USBManager::read_raw(unsigned char* read_array, int num){

        DWORD bytes_transferred;						// number of bytes that have been transferred 
		if (!(ReadFile(usb_handle, read_array, num, &bytes_transferred, nullptr))) {
            std::cerr << " failed\n";
        } 

        return bytes_transferred;
}

 
void USBManager::ClearInputBuffer() {
    // Nore, PURGE_RXABORT cancels all current read ops...
    PurgeComm(this->usb_handle, PURGE_RXCLEAR);
}

void USBManager::ClearOutputBuffer() {
    // Nore, PURGE_TXABORT cancels all current write ops...
    PurgeComm(this->usb_handle, PURGE_TXCLEAR);
}
