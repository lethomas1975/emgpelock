//
//  BTPeripheralView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct BTPeripheralView: View {
    var peripheral: BTPeripheral

    var body: some View {
        VStack(alignment: .leading) {
            HStack {
                Text("Name: \(peripheral.name)")
                Spacer()
            }
            HStack {
                Text("Identifier: \(peripheral.id)")
                Spacer()
            }
        }
    }
}

struct BTPeripheralView_Previews: PreviewProvider {
    static var previews: some View {
        BTPeripheralView(peripheral: BTPeripheral(id: UUID(uuidString: "1234")!, name: "Hello"))
    }
}
