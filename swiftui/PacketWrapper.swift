
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

import SwiftUI
import ReginaEngine

/**
 * Indidates one of the C++ classes designed for Swift that holds a packet of a particular type.
 *
 * Examples include regina.SharedContainer and regina.SharedLink.
 */
protocol SharedPacketClass {
    /**
     * A C++ shared pointer to the underlying packet type, such as `std::shared_ptr<regina::Container>` or
     * `std::shared_ptr<regina::PacketOf<regina::Link>>`.
     */
    associatedtype SharedPtr
    
    init(_: SharedPtr)
    init(_: regina.SharedPacket)
    func sharedPtr() -> SharedPtr
    func asPacket() -> regina.SharedPacket
}

/**
 * A SharedPacketClass where the underlying packet type holds some non-packet, value-based mathematical data.
 * Specifically, the C++ packet type here should be of the form `regina::PacketOf<...>`.
 *
 * Examples include regina.SharedLink, but _not_ regina.SharedContainer.
 */
protocol SharedHeldPacketClass: SharedPacketClass {
    /**
     * The C++ type holding the underlying mathematical data, such as `regina::Link`.
     */
    associatedtype Held
    
    init(_: Held)
    func heldCopy() -> Held
}

extension regina.SharedContainer: SharedPacketClass {}
extension regina.SharedText: SharedPacketClass {}

extension regina.SharedLink: SharedHeldPacketClass {}
extension regina.SharedSpatialLink: SharedHeldPacketClass {}

/**
 * An observable object that takes a packet upon initialisation, and then publishes
 * an "object will change" message every time that packet is about to change.
 *
 * Note: this class can only work with fixed types such as regina.SharedPacket or
 * PacketWrapper, not types that use generic parameters such as Wrapper<T>.
 * This is because Swift does not allow C callbacks to capture generic type arguments.
 */
class SwiftPacketChangeListener: ObservableObject {
    private var listener: regina.PacketChangeCallback
    
    init(packet: regina.SharedPacket) {
        self.listener = regina.PacketChangeCallback(packet.sharedPtr())

        // Unretained raw pointers to this object are fine here,
        // since this object's deinit() disables the callback.
        listener.enableCallbacks(Unmanaged<SwiftPacketChangeListener>.passUnretained(self).toOpaque())
        listener.callbackPacketToBeChanged = { context in
            Unmanaged<SwiftPacketChangeListener>.fromOpaque(context!).takeUnretainedValue().objectWillChange.send()
        }
    }
    
    deinit {
        listener.disableCallbacks()
    }
}

/**
 * A lightweight Swift wrapper around a C++ shared pointer to a packet of a specific type, which must _not_ be a null pointer.
 *
 * This is an ObservableObject, and it uses Regina's C++ PacketListener machinery to publish changes
 * to the object whenever the packet is about to change.
 *
 * Note: if view updates ever stop working, check out this note:
 * https://forums.swift.org/t/question-about-valid-uses-of-observableobject-s-synthesized-objectwillchange/31141
 * In particular, it may be necessary to cache a single publisher, e.g., via:
 * `let objectWillChange = ObservableObjectPublisher()`
 */
class Wrapper<T: SharedPacketClass>: SwiftPacketChangeListener, Equatable {
    let packet: T
    
    /**
     * PRE: \a packet is not a null pointer.
     */
    init(packet: T) {
        self.packet = packet
        super.init(packet: packet.asPacket())
    }
    
    /**
     * PRE: \a packet is not a null pointer.
     */
    override init(packet: regina.SharedPacket) {
        self.packet = .init(packet)
        super.init(packet: packet)
    }
    
    /**
     * PRE: \a wrapper is not a null pointer.
     * PRE: \a wrapper is a packet of the correct class.
     */
    init(wrapper: PacketWrapper) {
        self.packet = .init(wrapper.packet)
        super.init(packet: wrapper.packet)
    }
    
    /**
     * An integer that uniquely identifies this packet in the engine.
     *
     * Note that this is _not_ the same type used by PacketWrapper.id. Instead it is a plain 64-bit integer.
     * The C++ code in `engine/swift` includes a compile-time assertion to ensure that 64 bits is the correct size.
     *
     * If we ever need the ability to convert IDs back into packet wrappers then the type of this
     * property might need to change (to match for instance how PacketWrapper does things).
     */
    var id: Int64 {
        packet.asPacket().id()
    }

    /**
     * Identifies whether the two given wrappers refer to the same underlying packet.
     */
    static func == (lhs: Wrapper<T>, rhs: Wrapper<T>) -> Bool {
        // By comparing IDs, we are effectively comparing the raw C++ Packet pointers.
        return (lhs.packet.asPacket().id() == rhs.packet.asPacket().id())
    }
}

/**
 * A lightweight Swift wrapper around a C++ shared pointer to a generic packet (whose type is unknown), and which may be a null pointer.
 *
 * This is an ObservableObject, and it uses Regina's C++ PacketListener machinery to publish changes
 * to the object whenever the packet's children change.
 */
class PacketWrapper: ObservableObject, Identifiable, Equatable, Hashable {
    typealias ID = PacketWrapper
    
    /// A lightweight value-based C++ wrapper around the underlying packet.
    let packet: regina.SharedPacket
    
    private var listener: regina.PacketChildrenCallback

    /**
     * Creates a null packet wrapper.
     */
    init() {
        self.packet = .init()
        self.listener = .init()
    }

    /**
     * Creates a wrapper around the given C++ packet in Regina's calculation engine.
     *
     * This is safe to use even if _packet_ holds a null pointer.
     */
    init(packet: regina.SharedPacket) {
        self.packet = packet
        self.listener = .init(packet.sharedPtr())
        
        // Unretained raw pointers to this object are fine here,
        // since this object's deinit() disables the callback.
        listener.enableCallbacks(Unmanaged<PacketWrapper>.passUnretained(self).toOpaque())
        listener.callbackChildToBeAdded = { context in
            Unmanaged<PacketWrapper>.fromOpaque(context!).takeUnretainedValue().objectWillChange.send()
        }
        listener.callbackChildToBeRemoved = listener.callbackChildToBeAdded
        listener.callbackChildrenToBeReordered = listener.callbackChildToBeAdded
    }
    
    deinit {
        listener.disableCallbacks()
    }

    /**
     * Identifies whether the two given wrappers refer to the same underlying packet.
     *
     * All null packets are considerd equal to each other (but to nothing else).
     */
    static func == (lhs: PacketWrapper, rhs: PacketWrapper) -> Bool {
        // By comparing IDs, we are effectively comparing the raw C++ Packet pointers.
        return (lhs.packet.id() == rhs.packet.id())
    }
    
    /**
     * Returns a list of all immediate child packets of this packet.
     *
     * If this packet has no children or if this is a null packet, then this returns `nil`.
     */
    var children: [PacketWrapper]? {
        if packet.isNull() || packet.firstChild().isNull() {
            return nil
        }

        var ans: [PacketWrapper] = []
        var p = packet.firstChild()
        while !p.isNull() {
            ans.append(PacketWrapper(packet: p))
            p = p.nextSibling()
        }
        return ans
    }
    
    /**
     * Returns a 32-point image representing the type of the given packet.
     *
     * If this packet has an unknown type or if this is a null packet, then this returns `nil`.
     */
    var icon: Image? {
        if packet.isNull() {
            return nil
        }

        switch packet.type() {
        case .AngleStructures:
            return Image("Angles")
        case .Attachment:
            return Image("Attachment")
        case .Container:
            return Image("Container")
        case .Link:
            return Image("Link")
        case .NormalHypersurfaces:
            return Image("Hypersurfaces")
        case .NormalSurfaces:
            return Image("Surfaces")
        case .Script:
            return Image("Script")
        case .SnapPea:
            return Image("SnapPea")
        case .SpatialLink:
            return Image("SpatialLink")
        case .SurfaceFilter:
            return Image("Filter")
        case .Text:
            return Image("Text")
        case .Triangulation2:
            return Image("Triangulation2")
        case .Triangulation3:
            return Image("Triangulation3")
        case .Triangulation4:
            return Image("Triangulation4")
        case .Triangulation5:
            return Image("Triangulation5")
        case .Triangulation6:
            return Image("Triangulation6")
        case .Triangulation7:
            return Image("Triangulation7")
        case .Triangulation8:
            return Image("Triangulation8")
#if REGINA_HIGHDIM
        case .Triangulation9:
            return Image("Triangulation9")
        case .Triangulation10:
            return Image("Triangulation10")
        case .Triangulation11:
            return Image("Triangulation11")
        case .Triangulation12:
            return Image("Triangulation12")
        case .Triangulation13:
            return Image("Triangulation13")
        case .Triangulation14:
            return Image("Triangulation14")
        case .Triangulation15:
            return Image("Triangulation15")
#endif
        default:
            return nil
        }
    }
    
    @ViewBuilder var packetViewer: some View {
        if packet.isNull() {
            let msg = "Null packet"
            let detail = "This is a null packet, which should never occur.\nPlease report this to the Regina developers."
            HStack {
                Spacer()
                if #available(macOS 14.0, iOS 17.0, *) {
                    ContentUnavailableView {
                        Label(msg, systemImage: "exclamationmark.triangle")
                    } description: {
                        Text(detail)
                    }
                } else {
                    Text(detail)
                }
                Spacer()
            }
        } else {
            switch (packet.type()) {
            case .Link:
                LinkView(wrapper: Wrapper<regina.SharedLink>(wrapper: self))
            case .SpatialLink:
                SpatialLinkView(wrapper: Wrapper<regina.SharedSpatialLink>(packet: packet))
            case .Text:
                TextView(packet: regina.SharedText(packet))
            default:
                // TODO: Should we have a header with the packet name?
                let msg = "No viewer available"
                let detail = "I am not able to view packets of type \(swiftString(packet.typeName())) (yet).\nYou can, however, work with this packet through Regina's Python interface instead."
                HStack {
                    Spacer()
                    if #available(macOS 14.0, iOS 17.0, *) {
                        ContentUnavailableView {
                            Label(msg, systemImage: "info.circle")
                        } description: {
                            Text(detail)
                        }
                    } else {
                        Text(detail)
                    }
                    Spacer()
                }
            }
        }
    }
    
    /**
     * Returns a hashable ID that uniquely identifies the underlying packet in Regina's calculation engine.
     *
     * This is safe to use for null packets (and all null packets will receive equal IDs).
     */
    var id: ID { self }

    /**
     * Hashes this packet wrapper.
     * Different wrappers around the same underlying packet in Regina's calculation engine are guaranteed to receive the same hash.
     *
     * This is safe to use for null packets (and all null packets will receive the same hash value).
     */
    func hash(into hasher: inout Hasher) {
        hasher.combine(packet.id())
    }
}
