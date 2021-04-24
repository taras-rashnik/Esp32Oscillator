#if !defined(__BLEADAPTER_H__)
#define __BLEADAPTER_H__

#include <noncopyable.hpp>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "924c7d3c-a4da-11eb-bcbc-0242ac130002"
#define CHARACTERISTIC_UUID "a4154cd8-a4da-11eb-bcbc-0242ac130002"

class BleAdapter : boost::noncopyable
{
private:
public:
    BleAdapter() {}

    void setUp()
    {
        BLEDevice::init("BLE Oscilloscope");
        BLEServer *pServer = BLEDevice::createServer();
        BLEService *pService = pServer->createService(SERVICE_UUID);
        BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
                BLECharacteristic::PROPERTY_WRITE);

        pCharacteristic->setValue("Hello World says Neil");
        pService->start();
        // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
    }

    void tearDown() {}

    size_t write(const sample_t *pBuffer, size_t bufferSize) {}
};

#endif // __BLEADAPTER_H__
