#if !defined(__BLEADAPTER_H__)
#define __BLEADAPTER_H__

#include <noncopyable.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "924c7d3c-a4da-11eb-bcbc-0242ac130002"
#define CHARACTERISTIC_UUID "a4154cd8-a4da-11eb-bcbc-0242ac130002"

class BleAdapter : boost::noncopyable, public BLECharacteristicCallbacks, public BLEServerCallbacks
{
private:
    BLEServer *_pServer = nullptr;
    BLEService *_pService = nullptr;
    BLECharacteristic *_pCharacteristic = nullptr;
    BLEAdvertising *_pAdvertising = nullptr;

public:
    BleAdapter() {}

    size_t write(sample_t *pBuffer, size_t bufferSize)
    {
        _pCharacteristic->setValue(pBuffer, bufferSize);
        _pCharacteristic->indicate();
        return bufferSize;
    }

    void setUp()
    {
        tearDown();
        startService();
        startAdvertising();
    }

    void tearDown()
    {
        stopAdvertising();
        stopService();
    }

protected:
    virtual void onConnect(BLEServer *pServer)
    {
        Serial.println("BleAdapter::onConnect");
        // stopAdvertising();
    }

    virtual void onDisconnect(BLEServer *pServer)
    {
        Serial.println("BleAdapter::onDisconnect");
        // startAdvertising();
    }

    virtual void onRead(BLECharacteristic *pCharacteristic)
    {
        Serial.println("BleAdapter::onRead");
    }

    virtual void onWrite(BLECharacteristic *pCharacteristic)
    {
        Serial.println("BleAdapter::onWrite");
    }

private:
    void startService()
    {
        BLEDevice::init("BLE Oscilloscope");
        _pServer = BLEDevice::createServer();
        _pServer->setCallbacks(this);
        _pService = _pServer->createService(SERVICE_UUID);
        _pCharacteristic = _pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            (BLECharacteristic::PROPERTY_READ |
             BLECharacteristic::PROPERTY_WRITE |
             BLECharacteristic::PROPERTY_NOTIFY |
             BLECharacteristic::PROPERTY_INDICATE));

        // Create a BLE Descriptor
        _pCharacteristic->addDescriptor(new BLE2902());

        _pCharacteristic->setValue("Hello World says Neil 2");
        _pCharacteristic->setCallbacks(this);
        _pService->start();
    }

    void stopService()
    {
        if (_pService != nullptr)
        {
            _pService->stop();
            delete _pService;
            _pService = nullptr;
        }

        if (_pServer != nullptr)
        {
            _pServer->setCallbacks(nullptr);
            delete _pServer;
            _pServer = nullptr;
        }

        if (_pCharacteristic != nullptr)
        {
            _pCharacteristic->setCallbacks(nullptr);
            delete _pCharacteristic;
            _pCharacteristic = nullptr;
        }
    }

    void startAdvertising()
    {
        _pAdvertising = BLEDevice::getAdvertising();
        _pAdvertising->addServiceUUID(SERVICE_UUID);
        _pAdvertising->setScanResponse(true);
        _pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        _pAdvertising->setMinPreferred(0x12);
        _pAdvertising->start();
    }

    void stopAdvertising()
    {
        if (_pAdvertising != nullptr)
        {
            _pAdvertising->stop();
            delete _pAdvertising;
            _pAdvertising = nullptr;
        }
    }
};

#endif // __BLEADAPTER_H__
