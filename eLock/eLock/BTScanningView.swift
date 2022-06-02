//
//  BTScanningView.swift
//  eLock
//
//  Created by Thomas Le on 16/05/2022.
//

import SwiftUI

struct BTScanningView: View {
    @EnvironmentObject var appContext: AppContext
    @State var peripheralList: [BTPeripheral] = []
    @State var appeared = false
    @State var showView: Bool = false
    @State var progressMessage: String = ""
    
    var xOffset: CGFloat {
        showView ? 0 : -UIScreen.main.bounds.width/2
    }
    
    var yOffset: CGFloat {
        showView ? 40 : UIScreen.main.bounds.height
    }
    
    var body: some View {
        ZStack {
            VStack {
                Text("Bluetooth scanning...").fontWeight(Font.Weight.bold).font(Font.title)
                Spacer()
                List(peripheralList) { peripheral in
                    Button {
                        appContext.btManager.connect(peripheral: peripheral)
                    } label: {
                        BTPeripheralView(peripheral: peripheral)
                    }
                }
                .onReceive(appContext.btManager.$peripherals) { peripherals in
                    if peripherals.isEmpty {
                        showView = true
                    } else {
                        showView = false
                        peripheralList.removeAll(keepingCapacity: false)
                        peripherals.forEach { peripheral in
                            peripheralList.append(peripheral)
                        }
                    }
                }.onReceive(appContext.btManager.$connectedPeripheral) { connected in
                    if connected != nil && appeared {
                        Timer.scheduledTimer(withTimeInterval: 2.0, repeats: false) { timer in
                            appContext.appState = .CONNECTED
                        }
                    } else if appContext.appState != .CONNECTED && appContext.appState != .LOGGEDIN && appContext.appState != .MENU && appeared {
                        appContext.appState = .DISCONNECTED
                    }
                }.onAppear {
                    appeared = true
                    if appContext.appState != .CONNECTED {
                        let _ = appContext.btManager.connectToLastConnectedPeripheral()
                    }
                }.onDisappear {
                    appeared = false
                }
            }
            ScrollView {
                Spacer(minLength: 300)
                VStack(alignment: .center) {
                    ProgressView{
                        Text("").font(.title).fontWeight(.bold).frame(maxWidth: .infinity, maxHeight: .infinity)
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
}

struct BTScanningView_Previews: PreviewProvider {
    static var previews: some View {
        BTScanningView()
    }
}
