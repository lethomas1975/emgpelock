//
//  ToggleEncryptionDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class EncryptDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "C2+2"
    }
    
    func encrypt(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse) {
        let command = getCommand()
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
    
    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        stopTimer()
        if !message.isEmpty {
            print("encrypt status response: \(message)")
            if message.starts(with: "C2OK+E") {
                resetToMainDelegate(peripheral)
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
            } else {
                notifyFailure(message: "encrypt:nok")
            }
        }
    }
}
