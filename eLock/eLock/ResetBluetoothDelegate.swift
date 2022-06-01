//
//  ResetBluetoothDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class ResetBluetoothDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "C2+4"
    }
    
    func reset(peripheral: CBPeripheral, characteristic: CBCharacteristic, observer: CommandResponse) {
        let command = getCommand()
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
    
    override func receiveMessage(peripheral: CBPeripheral, characteristic: CBCharacteristic, message: String) {
        stopTimer()
        if !message.isEmpty {
            print("reset bluetooth response: \(message)")
            if message == "C2OK+RBT" {
                resetToMainDelegate(peripheral)
                notifySuccess(message: "rbt:ok")
            } else {
                notifyFailure(message: "rbt:nok")
            }
        }
    }
}
