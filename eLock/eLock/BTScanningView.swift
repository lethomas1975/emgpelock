//
//  BTScanningView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct BTScanningView: View {
    @EnvironmentObject var appContext: AppContext

    var body: some View {
        NavigationView {
            List(appContext.btManager.peripherals, id: \.identifier) { peripheral in
                NavigationLink {
                    PinView()
                } label: {
                    BTPeripheralView(peripheral: peripheral)
                }.onTapGesture() {
                    appContext.btManager.connect(peripheral: peripheral)
                }
            }
        }.onAppear {
            let connected = appContext.btManager.connectToLastConnectedPeripheral()
            if connected {
                appContext.appState = .CONNECTED
            }
        }
    }
}

struct BTScanningView_Previews: PreviewProvider {
    static var previews: some View {
        BTScanningView()
    }
}
