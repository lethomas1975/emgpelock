//
//  MenuView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct MenuView: View, CommandResponse, MessageObserver {
    @EnvironmentObject var appContext: AppContext

    @State var encrypted: Bool = AppContext.shared.encrypted
    @State var locked: Bool = AppContext.shared.locked
    @State var showView: Bool = false
    @State var progressMessage: String = ""
    @State var appeared = false
    @State var logText = ""
    
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
        if logText == "" {
            ResponseHandler.shared.addListener(self)
        }
        return ZStack {
            VStack {
                Text("Team C2: eLock Options").fontWeight(Font.Weight.bold).font(Font.title)
                Spacer()
                List {
                    Toggle(isOn: $locked, label: {
                        Text("Lock")
                    }).onTapGesture {
                        withAnimation {
                            showView.toggle()
                            progressMessage = "Toggling lock..."
                        }
                        appContext.btManager.sendLock(observer: self)
                    }
                    Button {
                        appContext.appState = .PINCHANGE
                    } label: {
                        Text("Change PIN")
                    }
                    Toggle(isOn: $encrypted, label: {
                        Text("Encrypt")
                    }).onTapGesture {
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
                    Button {
                        appContext.btManager.disconnectCurrentConnect()
                        appContext.appState = .DISCONNECTED
                    } label: {
                        Text("Exit")
                    }
                }
            }.blur(radius: blurRadius)
            ScrollView {
                VStack(alignment: .leading) {
                    Text(logText).frame(alignment: .leading).lineLimit(nil).multilineTextAlignment(.leading).font(.body.bold()).frame(maxWidth: .infinity, maxHeight: .infinity)
                }.frame(maxWidth: .infinity).padding(10).border(.foreground, width: 1)
            }.background(.black).foregroundColor(.white).offset(x: 0, y: 300).blur(radius: blurRadius)
            ScrollView {
                Spacer(minLength: 300)
                VStack(alignment: .center) {
                    ProgressView{
                        Text(progressMessage).font(.title).fontWeight(.bold).frame(maxWidth: .infinity, maxHeight: .infinity)
                    }
                }
            }.background(.black)
                .foregroundColor(.white)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
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
        } else if message == "lock:lock" {
            toggleProgressViewWithTimer("Locked!")
        } else if message == "lock:nok" {
            toggleProgressViewWithTimer("Lock toggle failed!")
        } else if message == "lock:unlock" {
            toggleProgressViewWithTimer("Unlocked!")
        } else if message == "rbt:ok" {
            toggleProgressViewWithTimer("Bluetooth reset!")
        } else if message == "rbt:nok" {
            toggleProgressViewWithTimer("Bluetooth reset failed!")
        } else if message == "encrypt:decrypt" {
            toggleProgressViewWithTimer("Encryption disabled!")
        } else if message == "encrypt:encrypt" {
            toggleProgressViewWithTimer("Encryption enabled!")
        }
    }
    
    func failed(_ message: String = "") {
        // TODO: show error message to user
        toggleProgressViewWithTimer("Command failed!")
    }
    
    private func toggleProgressViewWithTimer(_ message: String) {
        Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer in
            progressMessage = message
            Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer2 in
                showView.toggle()
            }
        }
    }
    
    func notify(_ message: String) {
        Timer.scheduledTimer(withTimeInterval: 0.5, repeats: false) { timer in
            if !logText.isEmpty {
                self.logText.append("\n")
            }
            self.logText.append("\(message)")
        }
    }
}

struct MenuView_Previews: PreviewProvider {
    static var previews: some View {
        MenuView()
    }
}
