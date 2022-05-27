//
//  MenuView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct MenuView: View, CommandResponse {
    @EnvironmentObject var appContext: AppContext

    @State var encrypted: Bool = false
    
    var body: some View {
        return VStack {
            Text("Team C2: eLock Options").fontWeight(Font.Weight.bold).font(Font.title)
            Spacer()
            List {
                Button {
                    appContext.appState = .LOCK
                } label: {
                    Text("Lock")
                }
                Button {
                    appContext.appState = .UNLOCK
                } label: {
                    Text("Unlock")
                }
                Button {
                    appContext.appState = .PINCHANGE
                } label: {
                    Text("Change PIN")
                }
                Toggle(isOn: $encrypted, label: {
                    Text("Encrypt")
                }).onChange(of: encrypted) { newValue in
                    appContext.btManager.sendEncrypt(observer: self)
                }
                Button {
                    appContext.appState = .RESET
                } label: {
                    Text("Reset Bluetooth")
                }
            }
        }.onAppear {
            appContext.btManager.sendEncryptStatus(observer: self)
        }
    }

    func successful(message: String = "") {
        if message.starts(with: "status:") {
            let index = message.index(message.endIndex, offsetBy: -1)
            let code = message[index...]
            if "0" == code {
                encrypted = false
            } else {
                encrypted = true
            }
        } else {
            appContext.btManager.sendEncryptStatus(observer: self)
        }
    }
    
    func failed(message: String = "") {
        // TODO: show error message to user
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
