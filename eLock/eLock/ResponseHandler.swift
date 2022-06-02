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
}
