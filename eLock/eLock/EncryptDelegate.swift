//
//  ToggleEncryptionDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class EncryptDelegate: BaseCommandDelegate {
    var state = 0;
    
    override func getCommand() -> String {
        return "AT+TYPE3"
    }
    
    func encrypt(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse?) {
        var command = getCommand()
        switch (state) {
        case 0:
            command = "AT+TYPE?"
        case 1:
            command = "AT+TYPE3"
        case 2:
            command = "AT+ERASE"
        case 3:
            command = "AT+TYPE0"
        case 4:
            command = "AT+RESET"
        default:
            command = getCommand()
        }
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
    
    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("pin response: \(message)")
            if state == 0 {
                if "OK+Get:0" == message {
                    state = 1
                    encrypt(peripheral: peripheral, characteristic: characteristic, observer: nil)
                } else if message.starts(with: "OK+Get:") {
                    state = 2
                    encrypt(peripheral: peripheral, characteristic: characteristic, observer: nil)
                } else {
                    notifyFailure(message: "encrypt:Can't get Pairing status!")
                }
            } else if state == 1 {
                if "OK+Set:3" == message {
                    state = 4
                    encrypt(peripheral: peripheral, characteristic: characteristic, observer: nil)
                } else {
                    notifyFailure(message: "encrypt:Can't set Auth and Bond!")
                }
            } else if state == 2 {
                if "OK+ERASE" == message {
                    state = 3
                    encrypt(peripheral: peripheral, characteristic: characteristic, observer: nil)
                } else {
                    notifyFailure(message: "encrypt:Can't set Auth and Bond!")
                }
            } else if state == 3 {
                if "OK+Set:0" == message {
                    state = 4
                    encrypt(peripheral: peripheral, characteristic: characteristic, observer: nil)
                } else {
                    notifyFailure(message: "encrypt:Can't set to No Code")
                }
            } else if state == 4 {
                if "OK+RESET" == message {
                    state = 0;
                    notifySuccess(message: "encrypt:ok")
                } else {
                    notifyFailure(message: "encrypt:Can't reset module")
                }
            }
        }
    }
}
