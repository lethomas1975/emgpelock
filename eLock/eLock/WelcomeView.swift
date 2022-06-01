//
//  WelcomeView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct WelcomeView: View {
    @EnvironmentObject var appContext: AppContext
    @State var appeared = false
    
    var body: some View {
        VStack(alignment: .center) {
            Text("Electronic Lock System").font(.title)
            Text("EMGP18PT122 Team C2").font(.subheadline)
            Text("June 2022").font(.subheadline)
        }.onReceive(appContext.btManager.$characteristic) { connected in
            if connected != nil && appeared {
                appContext.appState = .CONNECTED
            }
        }.onAppear() {
            appeared = true
            if appContext.appState != .CONNECTED  && appeared {
                Timer.scheduledTimer(withTimeInterval: 5.0, repeats: false) { timer in
                    if appContext.appState != .CONNECTED && appContext.appState != .LOGGEDIN && appContext.appState != .MENU && appeared {
                        appContext.appState = .DISCONNECTED
                    }
                }
            }
        }.onDisappear {
            appeared = false
        }
        
    }
}

struct WelcomeView_Previews: PreviewProvider {
    static var previews: some View {
        WelcomeView()
    }
}
