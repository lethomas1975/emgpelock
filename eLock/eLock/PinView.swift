//
//  PinView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI
import Introspect

struct PinView: View {
    @EnvironmentObject var appContext: AppContext

    @State var start = false
    @State var pin = ""
    @State var disabled = false
        
    var digits: Int = 3

    var dots: some View {
        HStack {
            ForEach(0..<digits, id:\.self) { index in
                Spacer()
                Image(systemName: imageName(index)).font(.system(size: 15, weight: .thin, design: .default))
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
            appContext.btManager.sendMessage("1")
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
        }/*.onReceive(appContext.btManager.$message) { message in
            if message.starts(with: "RXPIN:") {
                if message.hasSuffix("false") {
                    pin = ""
                    disabled = false
                } else if message.hasSuffix("true") {
                    appContext.appState = .LOGGEDIN
                }
            } else {
                pin = ""
                disabled = false
            }
        }*/
    }
}

struct PinView_Previews: PreviewProvider {
    static var previews: some View {
        PinView()
    }
}
