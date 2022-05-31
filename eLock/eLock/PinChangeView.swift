//
//  PinChangeView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI
import Introspect
struct PinChangeView: View, CommandResponse  {
    @EnvironmentObject var appContext: AppContext

    private let MAX_DIGITS = 3
    
    var xOffset: CGFloat {
        showView ? 0 : -UIScreen.main.bounds.width/2
    }
    
    var yOffset: CGFloat {
        showView ? 0 : UIScreen.main.bounds.height
    }
    
    var blurRadius: CGFloat {
        showView ? 20: 0
    }
    
    @State var showView: Bool = false
    @State var progressMessage: String = ""
    @State var oldPin: String = ""
    @State var newPin: String = ""
    @State var conPin: String = ""

    var body: some View {
        ZStack {
            VStack(alignment: .center, spacing: 10) {
                SecureField("Current PIN", text: $oldPin).keyboardType(.numberPad).onChange(of: oldPin) { newValue in
                    self.oldPin = String(newValue.prefix(MAX_DIGITS))
                }.padding(10)
                SecureField("New PIN", text: $newPin).keyboardType(.numberPad).onChange(of: newPin) { newValue in
                    self.newPin = String(newValue.prefix(MAX_DIGITS))
                }.padding(10)
                SecureField("Confirm PIN", text: $conPin).keyboardType(.numberPad).onChange(of: conPin) { newValue in
                    self.conPin = String(newValue.prefix(MAX_DIGITS))
                }.padding(10)
                Button {
                    withAnimation {
                        showView.toggle()
                        progressMessage = "Changing PIN..."
                    }
                    appContext.btManager.sendChangePin(oldPin: oldPin, newPin: newPin, confirmPin: conPin, observer: self)
                } label: {
                    Text("Change PIN")
                }
            }
            ProgressView{
                Text(progressMessage).font(.title).fontWeight(.bold).colorInvert()
            }.brightness(1)
                .padding(100)
                .animation(.spring(response: 0.75,
                                dampingFraction: 0.7,
                                blendDuration: 1), value: showView)
                .offset(x: xOffset, y: yOffset)
        }
    }
    
    func id() -> String {
        return "pinChangeView"
    }
    
    func successful(_ message: String) {
        toggleProgressViewWithTimer("Pin Changed!")
    }
    
    func failed(_ message: String) {
        oldPin = ""
        newPin = ""
        conPin = ""
        toggleProgressViewWithTimer("Pin change failed!", success: false)
    }
    
    private func toggleProgressViewWithTimer(_ message: String, success: Bool = true) {
        Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer in
            progressMessage = message
            Timer.scheduledTimer(withTimeInterval: 1.0, repeats: false) { timer2 in
                showView.toggle()
                if success {
                    appContext.appState = .LOGGEDIN
                }
            }
        }
    }
}

struct PinChangeView_Previews: PreviewProvider {
    static var previews: some View {
        PinChangeView()
    }
}
