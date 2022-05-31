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
    @State var showView: Bool = false
    @State var progressMessage: String = ""
    @State var appeared = false
    
    var xOffset: CGFloat {
        showView ? 0 : -UIScreen.main.bounds.width/2
    }
    
    var yOffset: CGFloat {
        showView ? 0 : UIScreen.main.bounds.height
    }
    
    var blurRadius: CGFloat {
        showView ? 20: 0
    }
    
    var body: some View {
        return ZStack {
            VStack {
                Text("Team C2: eLock Options").fontWeight(Font.Weight.bold).font(Font.title)
                Spacer()
                List {
                    Button {
                        withAnimation {
                            showView.toggle()
                            progressMessage = "Locking..."
                        }
                        appContext.btManager.sendLock(observer: self)
                    } label: {
                        Text("Lock")
                    }
                    Button {
                        withAnimation {
                            showView.toggle()
                            progressMessage = "Unlocking..."
                        }
                        appContext.btManager.sendUnlock(observer: self)
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
                        withAnimation {
                            showView.toggle()
                            progressMessage = "Toggling encryption..."
                        }
                        appContext.btManager.sendEncrypt(observer: self)
                    }
                    Button {
                        withAnimation {
                            showView.toggle()
                            progressMessage = "Resetting bluetooth..."
                        }
                        appContext.btManager.sendResetBluetooth(observer: self)
                    } label: {
                        Text("Reset Bluetooth")
                    }
                }
            }.blur(radius: blurRadius)
            ProgressView{
                Text(progressMessage).font(.title).fontWeight(.bold).colorInvert()
            }.brightness(1)
                .padding(100)
                .animation(.spring(response: 0.75,
                                dampingFraction: 0.7,
                                blendDuration: 1), value: showView)
                .offset(x: xOffset, y: yOffset)
        }.onReceive(appContext.btManager.$connectedPeripheral) { connected in
            if connected == nil && appeared {
                appContext.appState = .DISCONNECTED
            }
        }
        .onAppear {
            appeared = true
            appContext.btManager.sendEncryptStatus(observer: self)
        }.onDisappear {
            appeared = false
        }
    }

    func id() -> String {
        return "menuView"
    }

    func successful(_ message: String = "") {
        if message.starts(with: "status:") {
            let index = message.index(message.endIndex, offsetBy: -1)
            let code = String(message[index...])
            if "0" == code {
                encrypted = false
            } else {
                encrypted = true
            }
        } else if message == "lock:ok" {
            toggleProgressViewWithTimer("Locked!")
        } else if message == "lock:nok" {
            toggleProgressViewWithTimer("Lock failed!")
        } else if message == "unlock:ok" {
            toggleProgressViewWithTimer("Unlocked!")
        } else if message == "unlock:nok" {
            toggleProgressViewWithTimer("Unlock failed!")
        } else if message == "rbt:ok" {
            toggleProgressViewWithTimer("Bluetooth reset!")
        } else if message == "rbt:nok" {
            toggleProgressViewWithTimer("Bluetooth reset failed!")
        } else if message == "encrypt:ok" {
            toggleProgressViewWithTimer("Encryption toogled!")
        } else {
            appContext.btManager.sendEncryptStatus(observer: self)
        }
    }
    
    func failed(_ message: String = "") {
        // TODO: show error message to user
    }
    
    private func toggleProgressViewWithTimer(_ message: String) {
        Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer in
            progressMessage = message
            Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer2 in
                showView.toggle()
            }
        }
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
