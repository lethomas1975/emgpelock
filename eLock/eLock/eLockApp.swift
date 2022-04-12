//
//  eLockApp.swift
//  eLock
//
//  Created by Thomas Le on 12/04/2022.
//

import SwiftUI

@main
struct eLockApp: App {
    let persistenceController = PersistenceController.shared

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environment(\.managedObjectContext, persistenceController.container.viewContext)
        }
    }
}
