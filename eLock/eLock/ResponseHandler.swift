//
//  ResponseHandler.swift
//  eLock
//
//  Created by Thomas Le on 01/06/2022.
//

import Foundation

protocol MessageObserver {
    func notify(_ message: String);
}

class ResponseHandler {
    static let shared = ResponseHandler();
    
    private var messages: [String] = []
    
    private var listeners: [MessageObserver] = []
    
    func addListener(_ listener: MessageObserver) {
        listeners.append(listener)
        messages.forEach { message in
            listener.notify(message)
        }
    }
    
    func notify(_ message: String) {
        if !message.isEmpty {
            messages.append(message)
            listeners.forEach { listener in
                listener.notify(message)
            }
        }
    }
    
    func handleMessage(_ message: String) -> Void {
        if !message.isEmpty {
            if message ~= "^C2OK\\+E[01]\\+[LU]$" {
                AppContext.shared.encrypted = message[message.index(message.startIndex, offsetBy: 6)] == "1"
                AppContext.shared.locked = message[message.index(message.startIndex, offsetBy: 8)] == "L"
            } else if message ~= "^C2OK\\+E[01]$" {
                AppContext.shared.encrypted = message.last == "1"
            } else if message ~= "^C2OK\\+S[LU]$" {
                AppContext.shared.locked = message.last == "L"
            }
        }

    }
}
