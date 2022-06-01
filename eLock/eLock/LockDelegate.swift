//
//  LockDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class LockDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "C2+1"
    }
    
    func lock(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse) {
        let command = getCommand()
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
    
    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        stopTimer()
        if !message.isEmpty {
            print("lock status response: \(message)")
            if message.starts(with: "C2OK+SL") {
                AppContext.shared.locked = true
                resetToMainDelegate(peripheral)
                notifySuccess(message: "lock:lock")
            } else if message.starts(with: "C2OK+SU") {
                AppContext.shared.locked = false
                resetToMainDelegate(peripheral)
                notifySuccess(message: "lock:unlock")
            } else {
                notifyFailure(message: "lock:nok")
            }
        }
    }
}
