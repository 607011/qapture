/*
 Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
 All rights reserved.
 $Id: webcam.cpp 9e28cb30f3ba 2012/04/27 14:19:35 Oliver Lau <oliver@von-und-fuer-lau.de> $
*/

#include <windows.h>
#include <dshow.h>

#include "webcam.h"


Webcam::Webcam(void)
{
}


#pragma comment(lib, "strmiids")

HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
    // Create the System Device Enumerator.
    ICreateDevEnum *pDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
    if (SUCCEEDED(hr))
    {
        // Create an enumerator for the category.
        hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
        if (hr == S_FALSE)
        {
            hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
        }
        pDevEnum->Release();
    }
    return hr;
}


QVector<Webcam*> GetDeviceInformation(IEnumMoniker* pEnum)
{
    QVector<Webcam*> camList;
    IMoniker* pMoniker = NULL;
    while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
    {
        IPropertyBag *pPropBag;
        HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
        if (FAILED(hr))
        {
            pMoniker->Release();
            continue;
        }

        Webcam* cam = new Webcam;
        VARIANT var;
        VariantInit(&var);

        // Get description or friendly name.
        hr = pPropBag->Read(L"Description", &var, 0);
        if (FAILED(hr))
        {
            hr = pPropBag->Read(L"FriendlyName", &var, 0);
        }
        if (SUCCEEDED(hr))
        {
            // cam->setName();
            printf("%S\n", var.bstrVal);

            VariantClear(&var);
        }

        hr = pPropBag->Write(L"FriendlyName", &var);

        hr = pPropBag->Read(L"DevicePath", &var, 0);
        if (SUCCEEDED(hr))
        {
            // The device path is not intended for display.
            printf("Device path: %S\n", var.bstrVal);
            VariantClear(&var);
        }

        pPropBag->Release();
        pMoniker->Release();
        camList.append(cam);
    }
    return camList;
}


QVector<Webcam*> Webcam::enumerate(void)
{
    QVector<Webcam*> cams;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (SUCCEEDED(hr))
    {
        IEnumMoniker *pEnum;
        hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
        if (SUCCEEDED(hr))
        {
            cams = GetDeviceInformation(pEnum);
            pEnum->Release();
        }
        CoUninitialize();
    }
    return cams;
}
