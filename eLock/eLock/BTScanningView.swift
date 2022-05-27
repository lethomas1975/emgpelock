//
//  BTScanningView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct BTScanningView: View {
    @EnvironmentObject var appContext: AppContext
    @State var peripheralList: [BTPeripheral] = []
    
    var body: some View {
        VStack {
            Text("Bluetooth scanning...").fontWeight(Font.Weight.bold).font(Font.title)
            Spacer()
            List(peripheralList) { peripheral in
                Button {
                    appContext.btManager.connect(peripheral: peripheral)
                } label: {
                    BTPeripheralView(peripheral: peripheral)
                }
            }
            .onReceive(appContext.btManager.$peripherals) { peripherals in
                peripheralList.removeAll(keepingCapacity: false)
                peripherals.forEach { peripheral in
                    peripheralList.append(peripheral)
                }
            }.onReceive(appContext.btManager.$connectedPeripheral) { connected in
                if connected != nil {
                    Timer.scheduledTimer(withTimeInterval: 2.0, repeats: false) { timer in
                        appContext.appState = .CONNECTED
                    }
                } else {
                    appContext.appState = .DISCONNECTED
                }
            }
        }
    }
}

struct BTScanningView_Previews: PreviewProvider {
    static var previews: some View {
        BTScanningView()
    }
}
