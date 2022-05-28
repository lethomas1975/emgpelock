//
//  PinChangeDelegate.swift
//  eLock
//
//  Created by Thomas Le on 27/05/2022.
//

import Foundation
import CoreBluetooth

class PinChangeDelegate: BaseCommandDelegate {
    override func getCommand() -> String {
        return "C2+3"
    }
    
    func pin(peripheral: CBPeripheral, characteristic: CBCharacteristic, oldPin: String, newPin: String, confirmPin: String, observer: CommandResponse) {
        let command = "\(getCommand())+\(oldPin)+\(newPin)+\(confirmPin)"
        sendCommand(peripheral: peripheral, characteristic: characteristic, command: command, observer: observer)
    }
}