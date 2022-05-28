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
    @State var appeared = false
    
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
                if connected != nil && appeared {
                    Timer.scheduledTimer(withTimeInterval: 2.0, repeats: false) { timer in
                        appContext.appState = .CONNECTED
                    }
                } else if appContext.appState != .CONNECTED && appContext.appState != .LOGGEDIN && appContext.appState != .MENU && appeared {
                    appContext.appState = .DISCONNECTED
                }
            }.onAppear {
                appeared = true
                if appContext.appState != .CONNECTED {
                    let _ = appContext.btManager.connectToLastConnectedPeripheral()
                }
            }.onDisappear {
                appeared = false
            }
        }
    }
}

struct BTScanningView_Previews: PreviewProvider {
    static var previews: some View {
        BTScanningView()
    }
}
