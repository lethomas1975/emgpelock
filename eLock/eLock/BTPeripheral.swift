//
//  BTPeripheral.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import Foundation

struct BTPeripheral: Hashable, Codable, Identifiable {
    var id: UUID
    var name: String
}
