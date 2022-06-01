//
//  eLockApp.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

enum AppState: Int, CaseIterable {
    case START = 0, DISCONNECTED, CONNECTED, LOGGEDIN, MENU, PINCHANGE
}

class AppContext: ObservableObject {
    static let shared = AppContext()
    
    @Published var appState: AppState = .START
    @ObservedObject var btManager = HM10BTManager()
    @Published var locked: Bool = false
    @Published var encrypted: Bool = false
    
    func next() {
        if appState.rawValue < AppState.MENU.rawValue {
            appState = AppState(rawValue: appState.rawValue + 1)!
        }
    }
}

@main
struct eLockApp: App {
    let persistenceController = PersistenceController.shared
    @ObservedObject var appContext = AppContext.shared
    
    var body: some Scene {
        return WindowGroup {
            switch (appContext.appState) {
                case .START:
                    WelcomeView().environmentObject(appContext)
                case .DISCONNECTED:
                    BTScanningView().environmentObject(appContext)
                case .CONNECTED:
                    PinView().environmentObject(appContext)
                case .LOGGEDIN, .MENU:
                    MenuView().environmentObject(appContext)
                case .PINCHANGE:
                    PinChangeView().environmentObject(appContext)
            }
        }
    }
}
