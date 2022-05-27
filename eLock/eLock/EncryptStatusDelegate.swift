//
//  EncryptStatusDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class EncryptStatusDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "AT+TYPE?"
    }
    
    func status(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse) {
        let command = getCommand()
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
    
    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("encrypt status response: \(message)")
            if message.starts(with: "OK+Get:") {
                resetToMainDelegate(peripheral)
                let index = message.index(message.endIndex, offsetBy: -1)
                let code = message[index...]
                notifySuccess(message: "status:" + String(code))
            } else {
                notifyFailure()
            }
        }
    }
}
