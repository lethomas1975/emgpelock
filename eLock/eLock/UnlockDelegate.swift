//
//  UnlockDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class UnlockDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "C2+2"
    }

    func unlock(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse) {
        let command = getCommand()
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }

    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        if !message.isEmpty {
            print("unlock status response: \(message)")
            if message.starts(with: "C2OK+SU") {
                resetToMainDelegate(peripheral)
                notifySuccess(message: "unlock:ok")
            } else {
                notifyFailure(message: "unlock:nok")
            }
        }
    }
}
