//
//  HM10BTManager.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import Foundation
import CoreBluetooth
import CoreData
import SwiftUI

extension String {
    static func ~= (lhs: String, rhs: String) -> Bool {
        guard let regex = try? NSRegularExpression(pattern: rhs) else { return false }
        let range = NSRange(location: 0, length: lhs.utf8.count)
        return regex.firstMatch(in: lhs, options: [], range: range) != nil
    }

}

class HM10BTManager: NSObject, BluetoothListener, ObservableObject {
    private let viewContext = PersistenceController.shared.container.viewContext
    
    var hm10Bluetooth: HM10Bluetooth!
    
    var centralManager: CBCentralManager!
    @Published var connectedPeripheral: CBPeripheral!
    @Published var characteristic: CBCharacteristic!
    @Published var peripherals: [BTPeripheral] = []
    var cbPeripherals: [CBPeripheral] = []
    
    @Published var message: String = ""
    
    private var writeType: CBCharacteristicWriteType = .withoutResponse
    
    private let pinDelegate: PinDelegate = PinDelegate()
    private let lockDelegate: LockDelegate = LockDelegate()
    private let pinChangeDelegate: PinChangeDelegate = PinChangeDelegate()
    private let encryptDelegate: EncryptDelegate = EncryptDelegate()
    private let resetBluetoothDelegate: ResetBluetoothDelegate = ResetBluetoothDelegate()
    
    private let responseHandler: ResponseHandler = ResponseHandler.shared
    
    override init() {
        super.init()
        hm10Bluetooth = HM10Bluetooth(self)
        centralManager = CBCentralManager(delegate: hm10Bluetooth, queue: nil)
        pinDelegate.mainDelegate = self.hm10Bluetooth
    }
    
    func retriveCBPeripheral(_ btPeripheral: BTPeripheral) -> CBPeripheral? {
        var found: CBPeripheral?
        for cbPeripheral in cbPeripherals {
            if cbPeripheral.identifier == btPeripheral.id {
                found = cbPeripheral
                break
            }
        }
        return found
    }

    func connect(peripheral: BTPeripheral) {
        let cbPeripheral = retriveCBPeripheral(peripheral);
        if cbPeripheral != nil {
            hm10Bluetooth.stopScanning()
            hm10Bluetooth.connect(cbPeripheral!)
        }
    }

    func disconnect(peripheral: BTPeripheral) {
        let cbPeripheral = retriveCBPeripheral(peripheral);
        if cbPeripheral != nil {
            hm10Bluetooth.disconnect(cbPeripheral!)
        }
    }
    
    func disconnectCurrentConnect(_ forget: Bool = true) {
        if connectedPeripheral != nil {
            if forget {
                deletePeripheal(connectedPeripheral)
            }
            hm10Bluetooth.disconnect(connectedPeripheral)
        }
    }
    
    func readValue() {
        hm10Bluetooth.readValue()
    }
    
    // MARK: BluetoothListener functions
    func discoveredPeripheral(_ peripheral: CBPeripheral) {
        let name: String = (peripheral.name != nil) ? peripheral.name! : "Unknown"
        if !peripherals.contains(where: { $0.id.uuidString == peripheral.identifier.uuidString}) {
            peripherals.append(BTPeripheral(id: UUID(uuidString: peripheral.identifier.uuidString)!, name: name))
            cbPeripherals.append(peripheral)
            print("found \(name) with identifier \(peripheral.identifier.uuidString)")
            if let last = self.fetchLastConnected() {
                if last.identifier?.uuidString == peripheral.identifier.uuidString {
                    hm10Bluetooth.connect(peripheral)
                }
            }
        }
    }

    func connected(_ peripheral: CBPeripheral) {
        if peripherals.contains(where: { $0.id.uuidString == peripheral.identifier.uuidString}) {
            peripherals.removeAll(where: { $0.id.uuidString == peripheral.identifier.uuidString})
        }
        connectedPeripheral = peripheral
        do {
            if let result = fetch(peripheral.identifier) {
                result.lastConnected = Date()
                try viewContext.save()
            } else {
                let saved = savePeripheral(peripheral)
                if saved != nil {
                    saved?.lastConnected = Date()
                    try viewContext.save()
                }
            }
        } catch {
            // Replace this implementation with code to handle the error appropriately.
            // fatalError() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            let nsError = error as NSError
            fatalError("Unresolved error \(nsError), \(nsError.userInfo)")
        }
    }
    
    func disconnected(_ peripheral: CBPeripheral) {
        connectedPeripheral = nil
        do {
            let result = fetch(peripheral.identifier)
            if result != nil {
                result?.lastDisconnected = Date()
                try viewContext.save()
            }
        } catch {
            // Replace this implementation with code to handle the error appropriately.
            // fatalError() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            let nsError = error as NSError
            fatalError("Unresolved error \(nsError), \(nsError.userInfo)")
        }
    }
    
    func connectToLastConnectedPeripheral() -> Bool {
        let last = fetchLastConnected()
        if last == nil {
            return false
        }
        let peripheral = retriveCBPeripheral(BTPeripheral(id: last!.identifier!, name: last!.name!))
        if peripheral == nil {
            return false
        }
        hm10Bluetooth.connect(peripheral!)
        return true
    }

    func characteristic(_ characteristic: CBCharacteristic) {
        self.characteristic = characteristic
        writeType = characteristic.properties.contains(.write) ? .withResponse : .withoutResponse
        print("charateristic write type is \(writeType)")
    }
    
    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        print("receiving: \(message)")
        responseHandler.handleMessage(message)
    }
    
    // MARK: Persistence
    private func fetch(_ identifier: UUID) -> Peripheral? {
        let request: NSFetchRequest<Peripheral> = Peripheral.fetchRequest()
        request.predicate = NSPredicate(format: "%K == %@", "identifier", identifier as CVarArg)
        request.fetchLimit = 1
        
        do {
            return try viewContext.fetch(request).first
        } catch {
            // Replace this implementation with code to handle the error appropriately.
            // fatalError() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            let nsError = error as NSError
            fatalError("Unresolved error \(nsError), \(nsError.userInfo)")
        }
    }

    func fetchLastConnected() -> Peripheral? {
        let request: NSFetchRequest<Peripheral> = Peripheral.fetchRequest()
        request.fetchLimit = 1
        request.sortDescriptors = [NSSortDescriptor(key: "lastConnected", ascending: false)]
        
        do {
            return try viewContext.fetch(request).first
        } catch {
            // Replace this implementation with code to handle the error appropriately.
            // fatalError() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            let nsError = error as NSError
            fatalError("Unresolved error \(nsError), \(nsError.userInfo)")
        }

    }

    func savePeripheral(_ peripheral: CBPeripheral) -> Peripheral? {
        do {
            if fetch(peripheral.identifier) == nil {
                let toSave = Peripheral(context: viewContext)
                toSave.creationDate = Date()
                toSave.identifier = peripheral.identifier
                toSave.name = (peripheral.name != nil) ? peripheral.name! : "Unknown"
                try viewContext.save()
                return toSave
            }
            return nil
        } catch {
            // Replace this implementation with code to handle the error appropriately.
            // fatalError() causes the application to generate a crash log and terminate. You should not use this function in a shipping application, although it may be useful during development.
            let nsError = error as NSError
            fatalError("Unresolved error \(nsError), \(nsError.userInfo)")
        }
    }
    
    func deletePeripheal(_ peripheral: CBPeripheral) -> Void {
        if let per = fetch(peripheral.identifier) {
            viewContext.delete(per)
        }
    }

    // MARK: transmission functions
    // Send a string to the device
    func sendMessage(_ message: String) {
        print("sending: \(message)")
        if let data = message.data(using: String.Encoding.ascii) {
            hm10Bluetooth.write(value: data)
        }
    }
    
    func sendPin(_ pin: String, observer: CommandResponse) {
        print("sending pin: \(pin)")
        pinDelegate.pin(peripheral: connectedPeripheral, characteristic: characteristic, pin: encrypt(pin), observer: observer)
    }
    
    func sendLock(observer: CommandResponse) {
        print("sending lock command")
        lockDelegate.lock(peripheral: connectedPeripheral, characteristic: characteristic, observer: observer)
        
    }

    func sendChangePin(oldPin: String, newPin: String, confirmPin: String, observer: CommandResponse) {
        print("sending change pin command")
        pinChangeDelegate.pin(peripheral: connectedPeripheral, characteristic: characteristic, oldPin: encrypt(oldPin), newPin: encrypt(newPin), confirmPin: encrypt(confirmPin), observer: observer)
    }

    func sendEncrypt(observer: CommandResponse) {
        print("sending encrypt/decrypt command")
        encryptDelegate.encrypt(peripheral: connectedPeripheral, characteristic: characteristic, observer: observer)
    }

    func sendResetBluetooth(observer: CommandResponse) {
        print("sending reset command")
        resetBluetoothDelegate.reset(peripheral: connectedPeripheral, characteristic: characteristic, observer: observer)
    }
    
    private func encrypt(_ message: String) -> String {
        var encrypted = ""
        if AppContext.shared.encrypted {
            for (index, c) in message.enumerated() {
                encrypted += String(Character(UnicodeScalar(UInt8(Int(c.asciiValue!) + 1 + index))))
            }
        } else {
            encrypted = message
        }
        print("encrypted: \(encrypted)")
        return encrypted
    }
}
