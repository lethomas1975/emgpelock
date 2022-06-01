//
//  BaseCommandDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

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

class BaseCommandDelegate: NSObject, BluetoothListener, CBPeripheralDelegate {
    private var responseListeners: [CommandResponse] = [];

    var mainDelegate: CBPeripheralDelegate?
    
    var timer: Timer?
    
    func getCommand() -> String {
        fatalError("No Implementation")
    }
    
    func sendCommand(peripheral: CBPeripheral, characteristic: CBCharacteristic, command: String, observer: CommandResponse?) {
        addListener(observer);
        peripheral.delegate = self
        if let data = command.data(using: String.Encoding.ascii) {
            print("sending command: \(command)")
            ResponseHandler.shared.notify("sending: \(command)")
            write(peripheral: peripheral, characteristic: characteristic, value: data)
            timer = Timer.scheduledTimer(withTimeInterval: 5, repeats: false) { timer in
                if timer.isValid {
                    if AppContext.shared.btManager.connectedPeripheral != nil {
                        AppContext.shared.btManager.disconnectCurrentConnect(false)
                    }
                }
            }
        }
    }
    
    func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        stopTimer()
        if !message.isEmpty {
            print("Receiving response: \(message)")
            resetToMainDelegate(peripheral)
            notifySuccess()
        } else {
            notifyFailure()
        }
    }
    
    func stopTimer() {
        timer?.invalidate()
    }

    func write(peripheral: CBPeripheral, characteristic: CBCharacteristic, value: Data) {
        peripheral.writeValue(value, for: characteristic, type: .withResponse)
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        // notify the delegate in different ways
        // if you don't use one of these, just comment it (for optimum efficiency :])
        let data = characteristic.value
        guard data != nil else { return }
        
        // then the string
        if let str = String(data: data!, encoding: String.Encoding.ascii) {
            ResponseHandler.shared.notify("receiving: \(str)")
            receiveMessage(peripheral: peripheral, characteristic: characteristic, message: str)
        } else {
            print("Received an invalid string!")
        }
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
    
    func resetToMainDelegate(_ peripheral: CBPeripheral) {
        peripheral.delegate = mainDelegate
    }

    private func addListener(_ listener: CommandResponse? = nil) {
        if listener != nil && !responseListeners.contains(where: { $0.id() == listener?.id()}) {
            responseListeners.append(listener!)
        }
    }
}
