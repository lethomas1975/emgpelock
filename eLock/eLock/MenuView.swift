//
//  MenuView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct MenuView: View {
    @EnvironmentObject var appContext: AppContext

    var body: some View {
        appContext.appState = .MENU
        return Text("MenuView")
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
