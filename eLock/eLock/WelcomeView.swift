//
//  WelcomeView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct WelcomeView: View {
    @EnvironmentObject var appContext: AppContext
    
    var body: some View {
        VStack(alignment: .center) {
            Text("Electronic Lock System").font(.title)
            Text("EMGP18PT122 Team C2").font(.subheadline)
            Text("June 2022").font(.subheadline)
        }.onReceive(appContext.btManager.$peripherals) { newValue in
            if !newValue.isEmpty && appContext.appState != .CONNECTED {
                appContext.appState = .DISCONNECTED
            }
        }.onReceive(appContext.btManager.$connectedPeripheral) { connected in
            if connected != nil {
                appContext.appState = .CONNECTED
            }
        }
    }
}

struct WelcomeView_Previews: PreviewProvider {
    static var previews: some View {
        WelcomeView()
    }
}
