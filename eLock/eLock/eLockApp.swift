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
    
    let welcomeView = WelcomeView()
    let btScanningView = BTScanningView()
    let pinView = PinView()
    let menuView = MenuView()
    let pinChangeView = PinChangeView()
    
    var body: some Scene {
        return WindowGroup {
            switch (appContext.appState) {
                case .START:
                    welcomeView.environmentObject(appContext)
                case .DISCONNECTED:
                    btScanningView.environmentObject(appContext)
                case .CONNECTED:
                    pinView.environmentObject(appContext)
                case .LOGGEDIN, .MENU:
                    menuView.environmentObject(appContext)
                case .PINCHANGE:
                    pinChangeView.environmentObject(appContext)
            }
        }
    }
}
