//
//  PinView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI
import Introspect

struct PinView: View, CommandResponse {
    @EnvironmentObject var appContext: AppContext

    @State var pinFailed = false
    @State var pin = ""
    @State var disabled = false
        
    var digits: Int = 3

    var dots: some View {
        HStack {
            ForEach(0..<digits, id:\.self) { index in
                Spacer()
                Image(systemName: imageName(index)).font(.system(size: 15, weight: .thin, design: .default)).offset(x: pinFailed ? -10 : 0)
                    .animation(Animation.default.repeatCount(5), value: pinFailed)
            }
            Spacer()
        }
    }
    
    var pinField: some View {
        let hiddenPin = Binding<String>(
            get: {
                self.pin
            },
            set: { newValue in
                pinFailed = false
                self.pin = newValue
                self.checkPin()
            }
        )
        return TextField("", text: hiddenPin, onCommit: checkPin)
                .introspectTextField { textField in
                    textField.tintColor = .systemBackground
                    textField.textColor = .systemBackground
                    textField.keyboardType = .numberPad
                    textField.keyboardAppearance = .default
                    textField.becomeFirstResponder()
                    textField.isEnabled = !self.disabled
                    textField.isHidden = true
                }
    }
    
    func checkPin() {
        guard !pin.isEmpty else {
            return
        }
        
        if pin.count == digits && !disabled {
            disabled = true
            appContext.btManager.sendPin(pin, observer: self)
        }
    }
    
    func imageName(_ index: Int) -> String {
        if (index >= pin.count) {
            return "circle"
        }
        return "circle.fill"
    }

    var body: some View {
        VStack(alignment: .center) {
            Text("Enter PIN").font(.title)
            ZStack {
                dots.padding()
                pinField
            }
        }
    }
    
    func id() -> String {
        return "pinView"
    }
    
    func successful() {
        pin = ""
        disabled = false;
        appContext.appState = .LOGGEDIN
        pinFailed = false
    }
    
    func failed() {
        pin = ""
        disabled = false;
        pinFailed = true
    }

}

struct PinView_Previews: PreviewProvider {
    static var previews: some View {
        PinView()
    }
}
