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
        let range = NSRange(location: 0, length: lhs.count)
        return regex.firstMatch(in: lhs, options: [], range: range) != nil
    }

    var numberOfLines: Int {
            return self.components(separatedBy: "\n").count
    }
}

protocol CommandResponse {
    func id() -> String;
    func successful(_ message: String);
    func failed(_ message: String);
}

extension CommandResponse {
    func id() -> String {
        return ""
    }
    
    func successful(_ message: String = "") {}
    func failed(_ message: String = "") {}
}

class HM10BTManager: NSObject, BluetoothListener, ObservableObject {
    private let viewContext = PersistenceController.shared.container.viewContext
    
    private var timerCount: [ String: Int ] = [:]
    private var timers: [ String: Timer ] = [:]
    var hm10Bluetooth: HM10Bluetooth!
    
    var centralManager: CBCentralManager!
    @Published var connectedPeripheral: CBPeripheral!
    @Published var characteristic: CBCharacteristic!
    @Published var peripherals: [BTPeripheral] = []
    var cbPeripherals: [CBPeripheral] = []
    
    @Published var message: String = ""
    
    private var writeType: CBCharacteristicWriteType = .withoutResponse
    
    private var responseListeners: [CommandResponse] = [];

    private let responseHandler: ResponseHandler = ResponseHandler.shared
    
    override init() {
        super.init()
        hm10Bluetooth = HM10Bluetooth(self)
        centralManager = CBCentralManager(delegate: hm10Bluetooth, queue: nil)
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
    
    func sendStatusRequest() {
        print("sending status request")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2+0", observer: nil)
    }
    
    func sendPin(_ pin: String, observer: CommandResponse) {
        print("sending pin: \(pin)")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2PIN+" + encrypt(pin), observer: observer)
    }
    
    func sendLock(observer: CommandResponse) {
        print("sending lock command")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2+1", observer: observer)
    }

    func sendChangePin(oldPin: String, newPin: String, confirmPin: String, observer: CommandResponse) {
        print("sending change pin command")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2+3+\(encrypt(oldPin))+\(encrypt(newPin))+\(encrypt(confirmPin))", observer: observer)
    }

    func sendEncrypt(observer: CommandResponse) {
        print("sending encrypt/decrypt command")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2+2", observer: observer)
    }

    func sendResetBluetooth(observer: CommandResponse) {
        print("sending reset command")
        sendCommand(peripheral: connectedPeripheral, characteristic: characteristic, command: "C2+4", observer: observer)
    }
    
    private func encrypt(_ message: String) -> String {
        var encrypted = ""
        if AppContext.shared.encrypted {
            for (index, c) in message.enumerated() {
                encrypted += String(data: Data([UInt8(Int(c.asciiValue!) + 97 + index)]), encoding: String.Encoding.windowsCP1252)!
            }
            print("encrypted: \(encrypted)")
        } else {
            encrypted = message
            print("decrypted: \(encrypted)")
        }
        return encrypted
    }
    
    // MARK: BluetoothListener
    private func sendCommand(peripheral: CBPeripheral, characteristic: CBCharacteristic, command: String, observer: CommandResponse?) {
        addListener(observer);
        peripheral.delegate = hm10Bluetooth
        if let data = command.data(using: String.Encoding.windowsCP1252) {
            print("sending command: \(command)")
            ResponseHandler.shared.notify("sending: \(command)")
            write(peripheral: peripheral, characteristic: characteristic, value: data)
            if self.timerCount[command] == nil {
                self.timerCount[command] = 0
                
                self.timers[command] = Timer.scheduledTimer(withTimeInterval: 1, repeats: true) { timer in
                    if timer.isValid {
                        if self.timerCount[command] != nil && self.timerCount[command]! < 3 {
                            self.timerCount[command] = self.timerCount[command]! + 1
                            print("sending \(command) \(self.timerCount[command]!) times")
                            self.write(peripheral: peripheral, characteristic: characteristic, value: data)
                        } else if AppContext.shared.btManager.connectedPeripheral != nil {
                            if self.timerCount[command] != nil {
                                timer.invalidate()
                            }
                            self.timerCount[command] = nil
                            AppContext.shared.btManager.disconnectCurrentConnect(false)
                        }
                    }
                }
            }
        } else {
            notifyFailure(message: "Could not convert to ASCII")
        }
    }

    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("Receiving response: \(message)")
            if message ~= "^C2OK\\+E[01]\\+[LU]$" && message.count == 9 {
                let _ = invalidateTime("C2+0")
                AppContext.shared.encrypted = message[message.index(message.startIndex, offsetBy: 6)] == "1"
                AppContext.shared.locked = message[message.index(message.startIndex, offsetBy: 8)] == "L"
            } else if message ~= "^C2OK\\+S[LU]$" {
                let _ = invalidateTime("C2+1")
                AppContext.shared.locked = message.last == "L"
                if AppContext.shared.locked {
                    notifySuccess(message: "lock:lock")
                } else {
                    notifySuccess(message: "lock:unlock")
                }
            } else if message == "C2OK+CP" {
                let _ = invalidateTime("C2+3+")
                notifySuccess(message: "pin:cpok")
            } else if message == "C2NOK+CP" {
                let _ = invalidateTime("C2+3+")
                notifyFailure(message: "pin:cpnok")
            } else if "C2OK+PIN" == message {
                let _ = invalidateTime("C2PIN+")
                notifySuccess(message: "login:ok")
            } else if "C2NOK+PIN" == message {
                let _ = invalidateTime("C2PIN+")
                notifyFailure(message: "login:nok")
            } else if message ~= "^C2OK\\+E[01]$" {
                let _ = invalidateTime("C2+2")
                AppContext.shared.encrypted = message.last == "1"
                switch message.last {
                case "0":
                    AppContext.shared.encrypted = false
                    notifySuccess(message: "encrypt:decrypt")
                case "1":
                    AppContext.shared.encrypted = true
                    notifySuccess(message: "encrypt:encrypt")
                default:
                    notifyFailure(message: "encrypt:nok")
                }
            } else if message == "C2OK+RBT" {
                let _ = invalidateTime("C2+4")
                notifySuccess(message: "rbt:ok")
            } else if message == "C2NOK" {
                notifyFailure(message: message)
            } else {
                notifyFailure(message: "Unknown response")
            }
        } else {
            notifyFailure()
        }
    }
    
    private func invalidateTime(_ command: String, toNotify: Bool = false) -> Bool {
        var toNotifyResponse = false
        for (cmd, timer) in self.timers {
            if cmd.starts(with: command) {
                print("found timer with \(cmd) for \(command) to invalidate")
                if !(toNotify && self.timerCount[cmd] != nil && self.timerCount[cmd]! < 3) || !toNotify {
                    timer.invalidate();
                    self.timers[cmd] = nil
                    self.timerCount[cmd] = nil
                } else {
                    toNotifyResponse = true
                }
                break
            }
        }
        return toNotifyResponse
    }
    
    func write(peripheral: CBPeripheral, characteristic: CBCharacteristic, value: Data) {
        peripheral.writeValue(value, for: characteristic, type: .withResponse)
    }
    
    func notifySuccess(message: String = "") {
        responseListeners.forEach { response in
            response.successful(message)
        }
    }

    func notifyFailure(message: String = "") {
        responseListeners.forEach { response in
            response.failed(message)
        }
    }

    func addListener(_ listener: CommandResponse? = nil) {
        if listener != nil {
            if  responseListeners.contains(where: { $0.id() == listener?.id()}) {
                responseListeners.removeAll(where: { $0.id() == listener?.id()})
            }
            responseListeners.append(listener!)
        }
    }
}
