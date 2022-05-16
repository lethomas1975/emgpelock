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

class HM10BTManager: NSObject, BluetoothListener, ObservableObject {
    private let viewContext = PersistenceController.shared.container.viewContext
    
    var hm10Bluetooth: HM10Bluetooth!
    
    var centralManager: CBCentralManager!
    @Published var connectedPeripheral: CBPeripheral!
    @Published var peripherals: [BTPeripheral] = []
    var cbPeripherals: [CBPeripheral] = []
    
    override init() {
        super.init()
        hm10Bluetooth = HM10Bluetooth(self)
        centralManager = CBCentralManager(delegate: hm10Bluetooth, queue: nil)
    }
    
    func retriveCBPeripheral(_ btPeripheral: BTPeripheral) -> CBPeripheral? {
        var found: CBPeripheral?
        for cbPeripheral in cbPeripherals {
            if cbPeripheral.identifier == btPeripheral.identifier {
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
    
    // MARK: BluetoothListener functions
    func discoveredPeripheral(_ peripheral: CBPeripheral) {
        let name: String = (peripheral.name != nil) ? peripheral.name! : "Unknown"
        peripherals.append(BTPeripheral(identifier: UUID(uuidString: peripheral.identifier.uuidString)!, name: name))
        cbPeripherals.append(peripheral)
        print("found \(name) with identifier \(peripheral.identifier.uuidString)")
    }

    func connected(_ peripheral: CBPeripheral) {
        connectedPeripheral = peripheral
        do {
            let result = fetch(peripheral.identifier)
            if result != nil {
                result?.lastConnected = Date()
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
        let peripheral = retriveCBPeripheral(BTPeripheral(identifier: last!.identifier!, name: last!.name!))
        if peripheral == nil {
            return false
        }
        hm10Bluetooth.connect(peripheral!)
        return true
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

}
