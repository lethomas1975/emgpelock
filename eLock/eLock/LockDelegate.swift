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
}
