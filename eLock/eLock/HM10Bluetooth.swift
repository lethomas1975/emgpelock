//
//  HM10Bluetooth.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import Foundation
import CoreBluetooth
import SwiftUI

protocol BluetoothListener: AnyObject  {
    func discoveredPeripheral(_ peripheral: CBPeripheral)
    func connected(_ peripheral: CBPeripheral)
    func disconnected(_ peripheral: CBPeripheral)
    func connectToLastConnectedPeripheral() -> Bool
    func characteristic(_ characteristic: CBCharacteristic)
    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String)
}

extension BluetoothListener {
    func discoveredPeripheral(_ peripheral: CBPeripheral) {
    }
    
    func connected(_ peripheral: CBPeripheral) {
    }
    
    func disconnected(_ peripheral: CBPeripheral) {
    }
    
    func connectToLastConnectedPeripheral() -> Bool {
        return false
    }
    
    func characteristic(_ characteristic: CBCharacteristic) {
    }
    
    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
    }
}

class ConfigDelegate: NSObject, BluetoothListener, CBPeripheralDelegate {
    private var state = 0
    
    var mainDelegate: CBPeripheralDelegate?
    
    func startConfig(peripheral: CBPeripheral, characteristic: CBCharacteristic) {
        peripheral.delegate = self
        var command: String = ""
        switch (state) {
        case 0:
            command = "AT"
        case 1:
            command = "AT+NAME?"
        case 2:
            command = "AT+VERR?"
        case 3:
            command = "AT+BAUD0"
        default:
            peripheral.delegate = mainDelegate
            break
        }
        if !command.isEmpty, let data = command.data(using: String.Encoding.ascii) {
            print("config Sending: \(command)")
            write(peripheral: peripheral, characteristic: characteristic, value: data)
        }
    }
    
    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("config Receiving: \(message)")
            state += 1
            startConfig(peripheral: peripheral, characteristic: characteristic)
        }
    }

    func write(peripheral: CBPeripheral, characteristic: CBCharacteristic, value: Data) {
        peripheral.writeValue(value, for: characteristic, type: .withResponse)
        /*if peripheral.canSendWriteWithoutResponse {
            peripheral.writeValue(value, for: characteristic, type: .withoutResponse)
        } else {
            peripheral.writeValue(value, for: characteristic, type: .withResponse)
        }*/
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        // notify the delegate in different ways
        // if you don't use one of these, just comment it (for optimum efficiency :])
        let data = characteristic.value
        guard data != nil else { return }
        
        // then the string
        if let str = String(data: data!, encoding: String.Encoding.ascii) {
            receiveMessage(peripheral: peripheral, characteristic: characteristic, message: str)
        } else {
            print("Received an invalid string!")
        }
    }

}

class HM10Bluetooth: NSObject, CBCentralManagerDelegate, CBPeripheralManagerDelegate, CBPeripheralDelegate {
    static let HM_10_SERVICE_UUID = CBUUID(string: "FFE0")
    static let HM_10_CHARACTERISTIC_UUID = CBUUID(string: "FFE1")

    var centralManager: CBCentralManager!
    private var connectedPeripheral: CBPeripheral!
    private var characteristic: CBCharacteristic!
    
    private var listener: BluetoothListener
    
    var configDelegate: ConfigDelegate = ConfigDelegate()

    init(_ listener: BluetoothListener) {
        self.listener = listener;
        super.init()
        configDelegate.mainDelegate = self
    }
    
    func startScanning() -> Void {
        centralManager.scanForPeripherals(withServices: [HM10Bluetooth.HM_10_SERVICE_UUID])
    }
    
    func stopScanning() -> Void {
        centralManager?.stopScan()
    }
    
    func connect(_ peripheral: CBPeripheral) {
        centralManager.delegate = self
        centralManager?.connect(peripheral)
    }

    func disconnect(_ peripheral: CBPeripheral) {
        centralManager.delegate = self
        centralManager?.cancelPeripheralConnection(peripheral)
    }

    func readValue() {
        connectedPeripheral.delegate = self
        connectedPeripheral.readValue(for: characteristic)
    }
    
    func write(value: Data) {
        connectedPeripheral.delegate = self
        connectedPeripheral.writeValue(value, for: characteristic, type: .withResponse)
        /*if connectedPeripheral.canSendWriteWithoutResponse {
            connectedPeripheral.writeValue(value, for: characteristic, type: .withoutResponse)
        } else {
            connectedPeripheral.writeValue(value, for: characteristic, type: .withResponse)
        }*/
    }
    
    // MARK: CBCentralManagerDelegate functions
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if centralManager != central {
            centralManager = central
        }
        switch central.state {
            case .poweredOff:
                print("Is Powered Off.")
            case .poweredOn:
                print("Is Powered On.")
                startScanning()
            case .unsupported:
                print("Is Unsupported.")
            case .unauthorized:
                print("Is Unauthorized.")
            case .unknown:
                print("Unknown")
            case .resetting:
                print("Resetting")
            @unknown default:
                print("Error")
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        listener.discoveredPeripheral(peripheral)
    }

    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        listener.connected(peripheral)
        connectedPeripheral = peripheral
        connectedPeripheral.delegate = self
        connectedPeripheral.discoverServices([HM10Bluetooth.HM_10_SERVICE_UUID])
    }
    
    func centralManager(_ central: CBCentralManager, didDisconnectPeripheral peripheral: CBPeripheral, error: Error?) {
        listener.disconnected(peripheral)
    }
    
    func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        central.cancelPeripheralConnection(peripheral)
        listener.disconnected(peripheral)
    }
    
    // MARK: CBPeripheralDelegate functions
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        if ((error) != nil) {
            print("Error discovering services: \(error!.localizedDescription)")
            return
        }
        guard let services = peripheral.services else {
            return
        }
        //We need to discover the all characteristic
        print("Discovered Services: \(services)")
        for service in services {
            peripheral.discoverCharacteristics(nil, for: service)
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let characteristics = service.characteristics else {
            return
        }

        print("Found \(characteristics.count) characteristics.")
        for characteristic in characteristics {
            if characteristic.uuid.isEqual(HM10Bluetooth.HM_10_CHARACTERISTIC_UUID)  {
                print("Characteristic: \(characteristic.uuid)")
                self.characteristic = characteristic
                peripheral.setNotifyValue(true, for: characteristic)
                //configDelegate.startConfig(peripheral: peripheral, characteristic: characteristic)
                listener.characteristic(characteristic)
                if let data = "AT+VERR?".data(using: String.Encoding.ascii) {
                    print("sending: AT+VERR?")
                    write(value: data)
                }
                if let data = "AT+ANCS1".data(using: String.Encoding.ascii) {
                    print("sending: AT+ANCS1")
                    write(value: data)
                }
                if let data = "C2+0".data(using: String.Encoding.ascii) {
                    print("sending: C2+0")
                    write(value: data)
                }
                break;
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        let data = characteristic.value
        guard data != nil else { return }
        
        // then the string
        if let str = String(data: data!, encoding: String.Encoding.ascii) {
            listener.receiveMessage(peripheral: peripheral, characteristic: characteristic, message: str)
        } else {
            print("Received an invalid string!")
        }
    }

    // MARK: CBPeripheralManagerDelegate functions
    func peripheralManagerDidUpdateState(_ peripheral: CBPeripheralManager) {
        switch peripheral.state {
            case .poweredOn:
                print("Peripheral Is Powered On.")
            case .unsupported:
                print("Peripheral Is Unsupported.")
            case .unauthorized:
                print("Peripheral Is Unauthorized.")
            case .unknown:
                print("Peripheral Unknown")
            case .resetting:
                print("Peripheral Resetting")
            case .poweredOff:
                print("Peripheral Is Powered Off.")
            @unknown default:
                print("Error")
        }
    }

}
