//
//  PinView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct PinView: View, CommandResponse {
    @EnvironmentObject var appContext: AppContext
    
    private let MAX_DIGITS = 3
    
    @State var pin = ""
    @State var disabled = false
        
    @State var appeared = false
    @State private var showView: Bool = false
    @State var progressMessage: String = ""
    
    @FocusState var focus: Bool
    
    var xOffset: CGFloat {
        showView ? 0 : -UIScreen.main.bounds.width/2
    }
    
    var yOffset: CGFloat {
        showView ? 0 : UIScreen.main.bounds.height
    }
    
    var blurRadius: CGFloat {
        showView ? 20: 0
    }
    var digits: Int = 3

    var dots: some View {
        HStack {
            ForEach(0..<digits, id:\.self) { index in
                Spacer()
                Image(systemName: imageName(index)).font(.system(size: 15, weight: .thin, design: .default))
            }
            Spacer()
        }.background(.background)
    }
    
    var pinField: some View {
        return SecureField("", text: $pin).keyboardType(.numberPad).onChange(of: pin) { newValue in
            self.pin = String(newValue.prefix(MAX_DIGITS))
            self.checkPin()
        }.padding(0).foregroundColor(.clear).background(.clear).focused($focus).offset(x: -500, y: 0)
    }
    
    func checkPin() {
        guard !pin.isEmpty else {
            return
        }
        
        if self.pin.count == digits && !self.disabled {
            self.disabled = true
            print("check pin showView \(self.showView)")
            self.showView.toggle()
            print("check pin showView \(self.showView)")
            self.progressMessage = "Login..."
            self.appContext.btManager.sendPin(self.pin, observer: self)
        }
    }
    
    func imageName(_ index: Int) -> String {
        if (index >= pin.count) {
            return "circle"
        }
        return "circle.fill"
    }

    var body: some View {
        appContext.btManager.addListener(self)
        return ZStack {
            VStack(alignment: .center) {
                Text("Enter PIN").font(.title)
                ZStack {
                    self.dots.padding()
                    self.pinField
                }
            }.onReceive(appContext.btManager.$connectedPeripheral) { connected in
                if connected == nil && appeared {
                    self.appContext.appState = .DISCONNECTED
                    self.focus = true
                }
            }.onAppear {
                self.appeared = true
                self.focus = true
            }.onDisappear {
                self.appeared = false
            }
            ScrollView {
                Spacer(minLength: 300)
                VStack(alignment: .center) {
                    ProgressView{
                        Text(progressMessage).font(.title).fontWeight(.bold).frame(maxWidth: .infinity, maxHeight: .infinity)
                    }
                }
            }.background(.background)
                .foregroundColor(.primary)
                .frame(maxWidth: .infinity, maxHeight: .infinity)
                .animation(.spring(response: 0.75,
                                dampingFraction: 0.7,
                                blendDuration: 1), value: showView)
                .offset(x: xOffset, y: yOffset)
        }
    }
    
    func id() -> String {
        return "pinView"
    }
    
    func successful(_ message: String) {
        self.pin = ""
        print("success: \(self.appContext.appState) and message \(message)")
        if self.appContext.appState == .CONNECTED && message == "login:ok" {
            toggleViewIfFalse("Logged in!");
            self.toggleProgressViewWithTimer("Logged in!", success: true)
        }
    }
    
    func failed(_ message: String) {
        self.pin = ""
        print("failed: \(appContext.appState) and message \(message)")
        if self.appContext.appState == .CONNECTED && message == "login:nok" {
            toggleViewIfFalse("Login failed!");
            self.toggleProgressViewWithTimer("Login failed!", success: false)
        }
    }

    private func toggleViewIfFalse(_ message: String = "") {
        if showView == false {
            showView.toggle()
            progressMessage = message
        }
    }
    
    private func toggleProgressViewWithTimer(_ message: String, success: Bool) {
        Timer.scheduledTimer(withTimeInterval: 0.5, repeats: false) { timer in
            print("1st timer for message update \(message)")
            self.progressMessage = message
            print("1st timer for message update \(message)")
            Timer.scheduledTimer(withTimeInterval: 0.5, repeats: false) { timer2 in
                print("second timer for showView \(showView)")
                self.showView.toggle()
                print("second timer for showView \(showView)")
                if (success) {
                    self.pin = ""
                    self.disabled = false;
                    self.appContext.appState = .LOGGEDIN
                    self.focus = false
                } else {
                    self.disabled = false;
                    self.pin = ""
                    self.focus = true
                }
            }
        }
    }
}

struct PinView_Previews: PreviewProvider {
    static var previews: some View {
        PinView()
    }
}
