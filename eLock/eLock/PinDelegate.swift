//
//  PinDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class PinDelegate: BaseCommandDelegate {
    
    override func getCommand() -> String {
        return "C2PIN+"
    }
    
    func pin(peripheral: CBPeripheral, characteristic: CBCharacteristic, pin: String, observer: CommandResponse) {
        let command = getCommand() + pin
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }

    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("pin response: \(message)")
            if "C2OK+PIN" == message {
                resetToMainDelegate(peripheral)
                notifySuccess()
            } else {
                notifyFailure()
            }
        }
    }
}
