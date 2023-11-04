
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

struct PacketCell: View {
    static let iconSize = 36.0 // TODO: Choose the icon size properly
    var wrapper: PacketWrapper
    
    @State var opened = false

    var body: some View {
        HStack {
            if let icon = wrapper.icon {
                icon.resizable().frame(width: PacketCell.iconSize, height: PacketCell.iconSize)
            }
            VStack(alignment: .leading) {
                Text(String(wrapper.packet.humanLabel()))
                // TODO: Should we display child counts or not?
                let count = wrapper.packet.countChildren()
                if (count == 1) {
                    Text("1 subpacket").font(.footnote)
                } else if (count > 1) {
                    Text("\(count) subpackets").font(.footnote)
                }
            }
        }
    }
}

struct TreeView: View {
    // @ObservedObject var document: ReginaDocument
    var wrapper: PacketWrapper

    @State private var selected: PacketWrapper?

    init(packet: regina.SharedPacket) {
        wrapper = PacketWrapper(packet: packet)
    }

    var body: some View {
        NavigationSplitView {
            // We should not display the root packet.
            // Instead start directly with the list of top-level children.
            List(wrapper.children!, children: \.children, selection: $selected) { item in
                PacketCell(wrapper: item)
                // TODO: .listRowSeparator(.visible, edges: .bottom)
            }
            // TODO: Use filename for navigation title
            // .navigationTitle(String(wrapper.packet.humanLabel()))
        } detail: {
            // TODO: Packet viewer!
            if let s = selected {
                s.packetViewer
            }
        }
    }
}

struct TreeView_Previews: PreviewProvider {
    static var simpleTree: regina.SharedPacket {
        var root = regina.SharedPacket.makeContainer()

        var x = regina.SharedPacket.makeContainer()
        x.setLabel("First child")
        root.append(x)

        var y = regina.SharedPacket.makeContainer()
        y.setLabel("Second child")
        root.append(y)

        var y1 = regina.SharedPacket.makeContainer()
        y1.setLabel("Grandchild")
        y.append(y1)

        let z = regina.SharedPacket.makeContainer()
        root.append(z)

        return root
    }
    
    static var previews: some View {
        TreeView(packet: simpleTree)
    }
}
